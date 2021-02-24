#include "preview_widget.h"

// Application headers
#include "../dimensions.h"
#include "colour_widget.h"

// Qt headers
#include <QDir>
#include <QFileDialog>
#include <QFormLayout>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QSpinBox>

// --- Constructor --- //
PreviewWidget::PreviewWidget(QWidget *parent) : QWidget(parent)
{
    setMinimumWidth(Dimensions::WidgetPanelWidth + Dimensions::PreviewWidth);

    // Preview widget
    jersey_image_preview_ = new QLabel(this);
    jersey_image_preview_->setFixedSize(Dimensions::JerseyImageWidth, Dimensions::JerseyImageHeight);

    // Settings widget
    auto settings_widget{createSettingsWidget()};

    // Layout
    auto layout{new QHBoxLayout(this)};
    layout->addWidget(jersey_image_preview_);
    layout->addWidget(settings_widget);
}

// --- Create the settings widget --- //
QWidget *PreviewWidget::createSettingsWidget()
{
    auto widget{new QWidget(this)};

    // Colour selectors
    colour_selector_background_ = new ColourWidget(Jersey::defaultBackgroundColour(), widget);
    colour_selector_foreground_ = new ColourWidget(Jersey::defaultForegroundColour(), widget);
    colour_selector_trim_ = new ColourWidget(Jersey::defaultTrimColour(), widget);

    // Text Settings
    jersey_name_input_ = new QLineEdit("Smith", widget);
    jersey_number_input_ = new QSpinBox(widget);
    jersey_number_input_->setValue(88);
    jersey_number_input_->setRange(Jersey::MINIMUM_JERSEY_NUMBER, Jersey::MAXIMUM_JERSEY_NUMBER);

    // Buttons
    auto preview_button{new QPushButton(tr("Preview"), widget)};
    QObject::connect(preview_button, &QPushButton::clicked, this, &PreviewWidget::onPreview);

    auto save_button{new QPushButton(tr("Save as..."), widget)};
    QObject::connect(save_button, &QPushButton::clicked, this, &PreviewWidget::onSave);

    // Layout
    auto layout{new QFormLayout(widget)};
    layout->addRow(tr("Player surname:"), jersey_name_input_);
    layout->addRow(tr("Jersey number:"), jersey_number_input_);
    layout->addRow(tr("Background RGB:"), colour_selector_background_);
    layout->addRow(tr("Foreground RGB:"), colour_selector_foreground_);
    layout->addRow(tr("Trim RGB:"), colour_selector_trim_);
    layout->addRow(preview_button);
    layout->addRow(save_button);

    return widget;
}

// --- Generate the jersey --- //
Jersey PreviewWidget::generateJersey() const
{
    Jersey jersey(jersey_name_input_->text(), jersey_number_input_->value());
    jersey.setColours(colour_selector_background_->colour(),
                      colour_selector_foreground_->colour(),
                      colour_selector_trim_->colour());
    jersey.generate();
    jersey_image_preview_->setPixmap(jersey.pixmap());

    return jersey;
}

// --- Generate and display preview jersey --- //
void PreviewWidget::onPreview()
{
    generateJersey();
}

void PreviewWidget::onSave()
{
    const auto file_name{QFileDialog::getSaveFileName(this,
                                                      tr("Save Jersey Image"),
                                                      QString("%1/ehm_%2_%3.png")
                                                          .arg(QDir::homePath(),
                                                               jersey_name_input_->text(),
                                                               jersey_number_input_->text()),
                                                      tr("Images (*.png)"))};

    if (file_name.isEmpty())
        return;

    const auto jersey{generateJersey()};
    jersey.save(file_name);
}
