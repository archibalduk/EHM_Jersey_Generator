#include "preview_widget.h"

// Application headers
#include "../common/dimensions.h"
#include "../jersey/jersey_image_server.h"
#include "colour_widget.h"

// Qt headers
#include <QComboBox>
#include <QDir>
#include <QFileDialog>
#include <QFormLayout>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QRadioButton>
#include <QSpinBox>
#include <QVBoxLayout>

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

    // Initial jersey preview
    onPreview();
}

// --- Create the settings widget --- //
QWidget *PreviewWidget::createSettingsWidget()
{
    auto widget{new QWidget(this)};

    // Buttons
    auto preview_button{new QPushButton(tr("Preview"), widget)};
    QObject::connect(preview_button, &QPushButton::clicked, this, &PreviewWidget::onPreview);

    auto save_button{new QPushButton(tr("Save as..."), widget)};
    QObject::connect(save_button, &QPushButton::clicked, this, &PreviewWidget::onSave);

    // Layout
    auto layout{new QVBoxLayout(widget)};
    layout->addWidget(createTextDetailsGroup());
    layout->addWidget(createLayerSelectionGroup());
    layout->addWidget(createColourSelectionGroup());
    layout->addWidget(preview_button);
    layout->addWidget(save_button);

    return widget;
}

QGroupBox *PreviewWidget::createColourSelectionGroup()
{
    auto group{new QGroupBox(tr("Colours"), this)};

    // Colour selectors
    colour_selector_background_ = new ColourWidget(Jersey::defaultBackgroundColour(), group);
    colour_selector_foreground_ = new ColourWidget(Jersey::defaultForegroundColour(), group);
    colour_selector_trim_ = new ColourWidget(Jersey::defaultTrimColour(), group);

    auto layout{new QFormLayout(group)};
    layout->addRow(tr("Background RGB:"), colour_selector_background_);
    layout->addRow(tr("Foreground RGB:"), colour_selector_foreground_);
    layout->addRow(tr("Trim RGB:"), colour_selector_trim_);

    return group;
}

QGroupBox *PreviewWidget::createLayerSelectionGroup()
{
    auto group{new QGroupBox(tr("Jersey Design"), this)};

    // Designs/layers
    jersey_selector_foreground_ = new QComboBox(group);
    JerseyImageServer::foregroundLayers().setComboBox(jersey_selector_foreground_);
    QObject::connect(jersey_selector_foreground_,
                     &QComboBox::currentIndexChanged,
                     this,
                     &PreviewWidget::onPreview);

    jersey_selector_trim_ = new QComboBox(group);
    JerseyImageServer::trimLayers().setComboBox(jersey_selector_trim_);
    QObject::connect(jersey_selector_trim_,
                     &QComboBox::currentIndexChanged,
                     this,
                     &PreviewWidget::onPreview);

    jersey_selector_preset_ = new QComboBox(group);
    JerseyImageServer::presetImages().setComboBox(jersey_selector_preset_);
    QObject::connect(jersey_selector_preset_,
                     &QComboBox::currentIndexChanged,
                     this,
                     &PreviewWidget::onPreview);

    // Layer/preset options
    use_jersey_layer_images_ = new QRadioButton(tr("Use layer images"), group);
    use_jersey_preset_image_ = new QRadioButton(tr("Use preset image"), group);
    QObject::connect(use_jersey_layer_images_,
                     &QRadioButton::toggled,
                     this,
                     &PreviewWidget::onJerseyImageSettingChange);
    use_jersey_layer_images_->setChecked(true); // Layer images = default

    auto layout{new QFormLayout(group)};
    layout->addRow(use_jersey_layer_images_, use_jersey_preset_image_);
    layout->addRow(tr("Foreground layer:"), jersey_selector_foreground_);
    layout->addRow(tr("Trim layer:"), jersey_selector_trim_);
    layout->addRow(tr("Preset image:"), jersey_selector_preset_);

    return group;
}

QGroupBox *PreviewWidget::createTextDetailsGroup()
{
    auto group{new QGroupBox(tr("Text"), this)};

    // Text Settings
    jersey_name_input_ = new QLineEdit("Smith", group);
    jersey_number_input_ = new QSpinBox(group);
    jersey_number_input_->setValue(88);
    jersey_number_input_->setRange(Jersey::MINIMUM_JERSEY_NUMBER, Jersey::MAXIMUM_JERSEY_NUMBER);

    auto layout{new QFormLayout(group)};
    layout->addRow(tr("Player surname:"), jersey_name_input_);
    layout->addRow(tr("Jersey number:"), jersey_number_input_);

    return group;
}

// --- On jersey image layer/preset setting change --- //
void PreviewWidget::onJerseyImageSettingChange(const bool use_layers)
{
    jersey_selector_foreground_->setEnabled(use_layers);
    jersey_selector_trim_->setEnabled(use_layers);
    jersey_selector_preset_->setEnabled(!use_layers);
}

// --- Generate the jersey --- //
Jersey PreviewWidget::generateJersey() const
{
    Jersey jersey(jersey_name_input_->text(), jersey_number_input_->value());
    jersey.setColours(colour_selector_background_->colour(),
                      colour_selector_foreground_->colour(),
                      colour_selector_trim_->colour());
    jersey.setImages(jersey_selector_foreground_->currentIndex(),
                     jersey_selector_trim_->currentIndex(),
                     jersey_selector_preset_->currentIndex());
    jersey.usePresetImage(use_jersey_preset_image_->isChecked());
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
