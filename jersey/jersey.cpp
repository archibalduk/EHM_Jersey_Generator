#include "jersey.h"

// Application headers
#include "../dimensions.h"
#include "font_server.h"

// Qt headers
#include <QDebug>
#include <QImage>
#include <QPainter>
#include <QPen>
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
    // Background colour image
    const QImage background_layer{
        generateJerseyLayer(":/images/default_background.png", background_colour_)};

    // Foreground colour image
    const QImage foreground_layer{
        generateJerseyLayer(":/images/default_foreground.png", foreground_colour_)};

    // Trim colour image
    const QImage trim_layer{generateJerseyLayer(":/images/default_trim.png", trim_colour_)};

    // Font
    FontServer font_server;
    auto font{font_server.font()};

    // Paint/collate the jersey
    QPainter jersey_painter(image_);
    jersey_painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
    jersey_painter.setPen(pen());

    // Paint the jersey background layer
    jersey_painter.drawImage(0, Dimensions::JerseyImageVerticalPadding, background_layer);

    QSettings settings;

    // Two tone layer
    if (settings.value("two_tone_layer", false).toBool()) {
        const QImage two_tone_layer{":/images/two_tone_effect.png"};
        jersey_painter.setCompositionMode(QPainter::CompositionMode_Multiply);
        jersey_painter.drawImage(0, Dimensions::JerseyImageVerticalPadding, two_tone_layer);
        jersey_painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
    }

    // Paint the jersey foreground and trim layers
    jersey_painter.drawImage(0, Dimensions::JerseyImageVerticalPadding, foreground_layer);
    jersey_painter.drawImage(0, Dimensions::JerseyImageVerticalPadding, trim_layer);

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
    const QString name_text{
        (settings.value("upper_case_name_text", true).toBool()) ? surname_.toUpper() : surname_};

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

// --- Pen for use with text generation --- //
QPen Jersey::pen() const
{
    QPen pen(trim_colour_, Dimensions::PenThickness);
    pen.setCapStyle(Qt::RoundCap);
    pen.setJoinStyle(Qt::RoundJoin);

    return pen;
}
