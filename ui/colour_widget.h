#ifndef COLOUR_WIDGET_H
#define COLOUR_WIDGET_H

// Qt headers
#include <QColor>
class QSpinBox;
#include <QWidget>

// --- Colour selection widget --- //
class ColourWidget : public QWidget
{
public:
    // Constructor
    ColourWidget(const QColor &initial_colour, QWidget *parent = nullptr);

    // Get data
    QColor colour() const;

private:
    QSpinBox *spin_box_red_;
    QSpinBox *spin_box_green_;
    QSpinBox *spin_box_blue_;

    enum ENUM_FLAGS { RGB_MINUMUM = 0, RGB_MAXIMUM = 255 };
};

#endif // COLOURWIDGET_H
