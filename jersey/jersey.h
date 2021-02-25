#ifndef JERSEY_H
#define JERSEY_H

// Qt headers
#include <QColor>
#include <QFont>
class QImage;
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
    inline static QColor defaultBackgroundColour() { return QColor(193, 65, 66); }
    inline static QColor defaultForegroundColour() { return QColor(0, 0, 0); }
    inline static QColor defaultTrimColour() { return QColor(255, 255, 255); }

    // Set data
    inline void setColours(const QColor &background, const QColor &foreground, const QColor &trim)
    {
        background_colour_ = background;
        foreground_colour_ = foreground;
        trim_colour_ = trim;
    }
    inline void setJerseyNumber(const qint16 jersey_number) { jersey_number_ = jersey_number; }
    inline void setSurname(const QString &surname) { surname_ = surname; }

    enum ENUM_FLAGS { MINIMUM_JERSEY_NUMBER = 1, MAXIMUM_JERSEY_NUMBER = 99 };

private:
    // Colours
    QColor background_colour_;
    QColor foreground_colour_;
    QColor trim_colour_;

    // Image generation
    QImage generateJerseyLayer(const QString &file_name, QColor colour);
    QImage generateNameLayer() const;

    // Output image
    QImage *image_;

    // Pen
    QPen pen() const;

    // Player details
    qint16 jersey_number_;
    QString surname_;
};

#endif // JERSEY_H
