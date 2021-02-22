#include "colour_widget.h"

// Application headers
#include <QHBoxLayout>
#include <QSpinBox>

// --- Constructor --- //
ColourWidget::ColourWidget(const QColor &initial_colour, QWidget *parent) : QWidget(parent)
{
    spin_box_red_ = new QSpinBox(this);
    spin_box_red_->setRange(RGB_MINUMUM, RGB_MAXIMUM);
    spin_box_red_->setValue(initial_colour.red());

    spin_box_green_ = new QSpinBox(this);
    spin_box_green_->setRange(RGB_MINUMUM, RGB_MAXIMUM);
    spin_box_green_->setValue(initial_colour.green());

    spin_box_blue_ = new QSpinBox(this);
    spin_box_blue_->setRange(RGB_MINUMUM, RGB_MAXIMUM);
    spin_box_blue_->setValue(initial_colour.blue());

    auto layout{new QHBoxLayout(this)};
    layout->addWidget(spin_box_red_);
    layout->addWidget(spin_box_green_);
    layout->addWidget(spin_box_blue_);
}

// --- Get colour --- //
QColor ColourWidget::colour() const
{
    return QColor(spin_box_red_->value(), spin_box_green_->value(), spin_box_blue_->value());
}
