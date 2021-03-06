#include "jersey.h"

// Application headers
#include "common/dimensions.h"
#include "common/text.h"
#include "font_server.h"
#include "generic_design_server.h"
#include "jersey_image_server.h"
#include "team_design_server.h"

// Qt headers
#include <QImage>
#include <QPainter>
#include <QPen>
#include <QSettings>

// --- Static data --- //
std::default_random_engine Jersey::random_generator_(
    std::chrono::system_clock::now().time_since_epoch().count());

// --- Constructor --- //
Jersey::Jersey(const QString &surname, const qint32 jersey_number)
    : background_colour_(defaultBackgroundColour()), foreground_colour_(defaultForegroundColour()),
      trim_colour_((defaultTrimColour()))
{
    // Set data
    setJerseyNumber(jersey_number);
    setSurname(surname);

    // Output image
    image_ = new QImage(Dimensions::JerseyImageWidth,
                        Dimensions::JerseyImageHeight,
                        QImage::Format_ARGB32);
    image_->fill(QColor(0, 0, 0, 0)); // Fill the image in order to avoid artefacts
}

// --- Destructor --- //
Jersey::~Jersey()
{
    delete image_;
}

// --- File i/o --- //
bool Jersey::save(const QString &file_name) const
{
    QSettings settings;
    const auto image_quality{settings.value("image_quality", -1).toInt()};
    return image_->save(file_name, "png", image_quality);
}

// --- Generate image --- //
void Jersey::generate()
{
    // Font
    FontServer font_server;
    auto font{font_server.font()};

    // Paint/collate the jersey
    QPainter jersey_painter(image_);
    jersey_painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
    jersey_painter.setPen(pen());

    // Paint the layers or preset (as applicable)
    if (use_preset_image_)
        paintPresetImage(jersey_painter);
    else
        paintLayeredImage(jersey_painter);

    QSettings settings;

    // Paint the jersey name text
    auto name_layer{generateNameLayer()};
    jersey_painter.drawImage(0 + settings.value("name_text_horizontal_offset", 0).toInt(),
                             Dimensions::JerseyNameVerticalPosition
                                 + settings.value("name_text_vertical_offset", 0).toInt(),
                             name_layer.scaledToHeight(Dimensions::JerseyNameFontSize
                                                           + settings
                                                                 .value("name_text_size_offset", 0)
                                                                 .toInt(),
                                                       Qt::SmoothTransformation));

    // Paint the jersey number text
    font.setPointSize(Dimensions::JerseyNumberFontSize
                      + settings.value("number_text_size_offset", 0).toInt());
    jersey_painter.setFont(font);
    jersey_painter.drawText(0 + settings.value("number_text_horizontal_offset", 0).toInt(),
                            Dimensions::JerseyNumberVerticalPosition
                                + settings.value("number_text_vertical_offset", 0).toInt(),
                            Dimensions::JerseyImageWidth,
                            Dimensions::JerseyNumberFontSize,
                            Qt::AlignCenter,
                            QString::number(jersey_number_));

    jersey_painter.end();
}

// --- Set colours from strings --- //
void Jersey::setColours(const QString &background, const QString &foreground, const QString &trim)
{
    setColours(QColor(background.trimmed()), QColor(foreground.trimmed()), QColor(trim.trimmed()));

    // Check that none of the colours are invalid
    if (!background_colour_.isValid())
        background_colour_ = defaultBackgroundColour();
    if (!foreground_colour_.isValid())
        foreground_colour_ = defaultForegroundColour();
    if (!trim_colour_.isValid())
        trim_colour_ = defaultTrimColour();
}

// --- Set images based on club name --- //
void Jersey::setImages(const Text &club_name,
                       const GenericDesignServer &generic_jersey_designs,
                       const TeamDesignServer &team_jersey_designs)
{
    // FIRST: Check for a preset
    preset_image_id_ = JerseyImageServer::presetImages().find(club_name.simpleStringLowerCase());
    if (use_preset_image_ && preset_image_id_ > JerseyImageServer::NO_RESULT)
        return;

    use_preset_image_ = false; // Disable preset image if there was no preset image result

    // SECOND: Check for a defined team jersey layer design
    const auto team_jersey_design_id{team_jersey_designs.findTeam(club_name)};
    if (team_jersey_design_id != JerseyImageServer::NO_RESULT) {
        setImages(team_jersey_designs.foreground(team_jersey_design_id),
                  team_jersey_designs.trim(team_jersey_design_id),
                  JerseyImageServer::NO_RESULT);
        return;
    }

    // THIRD: Use a random jersey
    selectGenericLayersByClubName(club_name, generic_jersey_designs);

    // Purely random designs (could result in some weird designs)
    //selectPureRandomLayers();

    // Random designs based on club name (could result in some weird designs)
    //selectRandomLayersByClubName(club_name);
}

// --- Validate a colour value --- //
qint32 Jersey::createColourValue(const qint32 value)
{
    if (value > 255)
        return 255;
    if (value < 0)
        return 0;

    return value;
}

// --- Select a random generic jersey design by club name --- //
void Jersey::selectGenericLayersByClubName(const Text &club_name,
                                           const GenericDesignServer &generic_jersey_designs)
{
    const auto club_name_pct{club_name.averageCharacterValuePercentage()};

    setImages(generic_jersey_designs.findForeground(club_name_pct),
              generic_jersey_designs.findTrim(club_name_pct),
              JerseyImageServer::NO_RESULT);
}

// --- Select entirely random jerseys (likely to result in weird combinations) --- //
void Jersey::selectPureRandomLayers()
{
    std::uniform_int_distribution<qint32>
        foreground_distribution(0, JerseyImageServer::foregroundLayers().size());
    std::uniform_int_distribution<qint32> trim_distribution(0,
                                                            JerseyImageServer::trimLayers().size());

    setImages(foreground_distribution(random_generator_),
              trim_distribution(random_generator_),
              JerseyImageServer::NO_RESULT);
}

// --- Select random layers based on the club name --- //
void Jersey::selectRandomLayersByClubName(const Text &club_name)
{
    const auto club_name_pct{club_name.averageCharacterValuePercentage()};
    qInfo() << club_name.text() << club_name_pct;
    setImages(JerseyImageServer::foregroundLayers().find(club_name_pct),
              JerseyImageServer::trimLayers().find(club_name_pct),
              JerseyImageServer::NO_RESULT);
}

// --- Generate a layer of the jersey image --- //
QImage Jersey::generateJerseyLayer(const QString &file_name, QColor colour)
{
    QImage image(file_name);

    const auto height{image.height()};
    const auto width{image.width()};

    // Loop all the pixels
    for (qint32 y = 0; y < height; ++y) {
        for (qint32 x = 0; x < width; ++x) {
            // Read the alpha value each pixel, keeping the RGB values of your color
            colour.setAlpha(image.pixelColor(x, y).alpha());

            // Apply the pixel color
            image.setPixelColor(x, y, colour);
        }
    }

    return image;
}

// --- Generate the name layer of the image --- //
QImage Jersey::generateNameLayer() const
{
    /* The text quality generated appears rather poor at small sizes.
     * This function generates a larger image when can then be scaled down,
     * resulting better quality text.
     */

    // Font
    FontServer font_server;
    auto font{font_server.font()};
    font.setPointSize(Dimensions::JerseyNameUpscaledFontSize);

    // Name text
    QSettings settings;
    QString name_text{(settings.value("upper_case_name_text", true).toBool()) ? surname_.toUpper()
                                                                              : surname_};

    if (!settings.value("accented_characters", false).toBool()) // Strip accented characters
        Text::toSimpleString(name_text);

    // Name layer image
    QImage name_layer(Dimensions::JerseyNameUpscaledWidth,
                      Dimensions::JerseyNameUpscaledHeight,
                      QImage::Format_ARGB32);
    name_layer.fill(QColor(0, 0, 0, 0)); // Fill the image in order to avoid artefacts

    // Name layer painter
    QPainter painter(&name_layer);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
    painter.setPen(pen());
    painter.setFont(font);
    painter.drawText(0,
                     0,
                     Dimensions::JerseyNameUpscaledWidth,
                     Dimensions::JerseyNameUpscaledHeight,
                     Qt::AlignCenter,
                     name_text);
    painter.end();

    return name_layer;
}

// --- Paint/generate the jersey using layer images --- //
void Jersey::paintLayeredImage(QPainter &jersey_painter)
{
    // Background colour image
    const QImage background_layer{generateJerseyLayer(JerseyImageServer::backgroundLayers().fileName(
                                                          background_layer_image_id_),
                                                      background_colour_)};

    // Foreground colour image
    const QImage foreground_layer{generateJerseyLayer(JerseyImageServer::foregroundLayers().fileName(
                                                          foreground_layer_image_id_),
                                                      foreground_colour_)};

    // Trim colour image
    const QImage trim_layer{
        generateJerseyLayer(JerseyImageServer::trimLayers().fileName(trim_layer_image_id_),
                            trim_colour_)};

    // Paint the jersey background layer
    jersey_painter.drawImage(0, Dimensions::JerseyImageVerticalPadding, background_layer);

    QSettings settings;

    // Two tone layer
    if (settings.value("two_tone_layer", false).toBool()) {
        const auto two_tone_red{createColourValue(background_colour_.red() + 30)};
        const auto two_tone_green{createColourValue(background_colour_.green() + 28)};
        const auto two_tone_blue{createColourValue(background_colour_.blue() + 23)};

        const QColor two_tone_colour{two_tone_red, two_tone_green, two_tone_blue};

        const QImage two_tone_layer{
            generateJerseyLayer(":/images/two_tone_effect.png", two_tone_colour)};
        jersey_painter.drawImage(0, Dimensions::JerseyImageVerticalPadding, two_tone_layer);
    }

    // Paint the jersey foreground and trim layers
    jersey_painter.drawImage(0, Dimensions::JerseyImageVerticalPadding, foreground_layer);
    jersey_painter.drawImage(0, Dimensions::JerseyImageVerticalPadding, trim_layer);
}

// --- Paint/generate the jersey using a preset image --- //
void Jersey::paintPresetImage(QPainter &jersey_painter)
{
    const QImage preset_image(JerseyImageServer::presetImages().fileName(preset_image_id_));
    jersey_painter.drawImage(0,
                             Dimensions::JerseyImageVerticalPadding,
                             preset_image.scaledToHeight(Dimensions::JerseyImageHeight
                                                             - Dimensions::JerseyImageVerticalPadding,
                                                         Qt::SmoothTransformation));
}

// --- Pen for use with text generation --- //
QPen Jersey::pen() const
{
    QPen pen(trim_colour_, Dimensions::PenThickness);
    pen.setCapStyle(Qt::RoundCap);
    pen.setJoinStyle(Qt::RoundJoin);

    return pen;
}
