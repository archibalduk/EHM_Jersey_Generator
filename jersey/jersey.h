#ifndef JERSEY_H
#define JERSEY_H

// Application headers
class GenericDesignServer;
class TeamDesignServer;
class Text;

// Qt headers
#include <QColor>
#include <QFont>
class QImage;
#include <random>
#include <QPen>
#include <QPixmap>
#include <QString>

// --- Jersey image generator --- //
class Jersey
{
public:
    // Constructor
    Jersey(const QString &surname = "Smith", const qint32 jersey_number = 99);
    ~Jersey();

    // File I/O
    bool save(const QString &file_name) const;

    // Image generation
    void generate();

    // Get data
    inline QPixmap pixmap() const { return QPixmap::fromImage(*image_); }

    // Get default colour values
    inline static QColor defaultBackgroundColour() { return QColor(10, 40, 70); }
    inline static QColor defaultForegroundColour() { return QColor(176, 0, 0); }
    inline static QColor defaultTrimColour() { return QColor(255, 208, 0); }

    // Set data
    void setColours(const QString &background, const QString &foreground, const QString &trim);
    inline void setColours(const QColor &background, const QColor &foreground, const QColor &trim)
    {
        background_colour_ = background;
        foreground_colour_ = foreground;
        trim_colour_ = trim;
    }
    inline void setJerseyNumber(const qint16 jersey_number) { jersey_number_ = jersey_number; }
    void setImages(const Text &club_name,
                   const GenericDesignServer &generic_jersey_designs,
                   const TeamDesignServer &team_jersey_designs);
    inline void setImages(const qint32 foreground_layer_image,
                          const qint32 trim_layer_image,
                          const qint32 preset_image)
    {
        foreground_layer_image_id_ = foreground_layer_image;
        trim_layer_image_id_ = trim_layer_image;
        preset_image_id_ = preset_image;
    }
    inline void setSurname(const QString &surname) { surname_ = surname; }
    inline void usePresetImage(const bool b) { use_preset_image_ = b; }

    enum ENUM_FLAGS { MINIMUM_JERSEY_NUMBER = 1, MAXIMUM_JERSEY_NUMBER = 99 };

private:
    // Colours
    qint32 createColourValue(const qint32 value);
    QColor background_colour_;
    QColor foreground_colour_;
    QColor trim_colour_;

    // Generic image selection
    static std::default_random_engine random_generator_;
    void selectGenericLayersByClubName(const Text &club_name,
                                       const GenericDesignServer &generic_jersey_designs);
    void selectPureRandomLayers();
    void selectRandomLayersByClubName(const Text &club_name);

    // Image generation
    QImage generateJerseyLayer(const QString &file_name, QColor colour);
    QImage generateNameLayer() const;
    void paintLayeredImage(QPainter &jersey_painter);
    void paintPresetImage(QPainter &jersey_painter);

    // Layer image designs
    qint32 background_layer_image_id_{0};
    qint32 foreground_layer_image_id_{0};
    qint32 trim_layer_image_id_{0};
    qint32 preset_image_id_{0};
    bool use_preset_image_{false};

    // Output image
    QImage *image_;

    // Pen
    QPen pen() const;

    // Player details
    qint16 jersey_number_;
    QString surname_;
};

#endif // JERSEY_H
