#include "jersey.h"

// Application headers
#include "../dimensions.h"
#include "font_server.h"
#include "jersey_image_server.h"

// Qt headers
#include <QDebug>
#include <QImage>
#include <QPainter>
#include <QPen>
#include <QRegularExpression>
#include <QSettings>

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
    return image_->save(file_name);
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
        toSimpleString(name_text);

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

    qInfo() << QString("Upscaled name layer dimensions: %1 x %2")
                   .arg(name_layer.width())
                   .arg(name_layer.height());
    qInfo() << QString("Downscaled name layer dimensions: %1 x %2")
                   .arg(name_layer.width() / Dimensions::JerseyNameUpscaleMultiplier)
                   .arg(name_layer.height() / Dimensions::JerseyNameUpscaleMultiplier);

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
        const QColor two_tone_colour{background_colour_.red() + 30,
                                     background_colour_.green() + 28,
                                     background_colour_.blue() + 23};

        const QImage two_tone_layer{
            generateJerseyLayer(":/images/two_tone_effect.png", two_tone_colour)};
        // jersey_painter.setCompositionMode(QPainter::CompositionMode_ColorDodge);
        jersey_painter.drawImage(0, Dimensions::JerseyImageVerticalPadding, two_tone_layer);
        //jersey_painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
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

void Jersey::toSimpleString(QString &text) const
{
    text = text.trimmed();

    // Remove characters to be ignored
    text.remove(QChar(0x2122)); // Trademark

    // Replace characters not corrected by normalising
    text.replace("ø", "o");

    // Remove accents and convert for Latin-1
    text = text.normalized(QString::NormalizationForm_KD)
               .remove(QRegularExpression("^a-zA-Z\\s\\d_:"))
               .toLatin1();

    // Remove any question marks left over from conversion to Latin-1
    text = text.remove("?");
}
