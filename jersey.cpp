#include "jersey.h"

// Application headers
#include "dimensions.h"

// Qt headers
#include <QDebug>
#include <QFontDatabase>
#include <QImage>
#include <QPainter>
#include <QPen>

// --- Static members --- //
bool Jersey::use_uppercase_text_ = true;

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
bool Jersey::save(const QString &filename) const
{
    return image_->save(filename);
}

// --- Generate font --- //
QFont Jersey::font() const
{
    const auto font_id{QFontDatabase::addApplicationFont(":/fonts/sports_jersey.ttf")};
    const auto font_family{QFontDatabase::applicationFontFamilies(font_id).at(0)};

    QFont font(font_family, Dimensions::JerseyNameFontSize, QFont::Thin);
    font.setLetterSpacing(QFont::AbsoluteSpacing, Dimensions::FontLetterSpacing);
    font.setStyleStrategy(QFont::PreferAntialias);
    font.setStretch(QFont::Unstretched);

    return font;
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
    auto font{this->font()};

    // Paint/collate the jersey
    QPainter jersey_painter(image_);
    jersey_painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
    jersey_painter.setPen(pen());

    // Paint the jersey layers
    jersey_painter.drawImage(0, Dimensions::JerseyImageVerticalPadding, background_layer);
    jersey_painter.drawImage(0, Dimensions::JerseyImageVerticalPadding, foreground_layer);
    jersey_painter.drawImage(0, Dimensions::JerseyImageVerticalPadding, trim_layer);

    // Paint the jersey name text
    auto name_layer{generateNameLayer()};
    jersey_painter.drawImage(0,
                             Dimensions::JerseyNameVerticalPosition,
                             name_layer.scaledToHeight(Dimensions::JerseyNameFontSize,
                                                       Qt::SmoothTransformation));

    // Paint the jersey number text
    font.setPointSize(Dimensions::JerseyNumberFontSize);
    jersey_painter.setFont(font);
    jersey_painter.drawText(0,
                            Dimensions::JerseyNumberVerticalPosition,
                            Dimensions::JerseyImageWidth,
                            Dimensions::JerseyNumberFontSize,
                            Qt::AlignCenter,
                            QString::number(jersey_number_));

    jersey_painter.end();
}

// --- Generate a layer of the jersey image --- //
QImage Jersey::generateJerseyLayer(const QString &filename, QColor colour)
{
    QImage image(filename);

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
    auto font{this->font()};
    font.setPointSize(Dimensions::JerseyNameUpscaledFontSize);

    // Name text
    const QString name_text{(use_uppercase_text_) ? surname_.toUpper() : surname_};

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
