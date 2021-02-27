#ifndef PREVIEW_WIDGET_H
#define PREVIEW_WIDGET_H

// Application headers
class ColourWidget;
#include "../jersey/jersey.h"

// Qt headers
class QComboBox;
class QGroupBox;
class QLabel;
class QLineEdit;
class QRadioButton;
class QSpinBox;
#include <QWidget>

// --- Jersey preview widget --- //
class PreviewWidget : public QWidget
{
public:
    PreviewWidget(QWidget *parent = nullptr);

private:
    // Colour settings
    ColourWidget *colour_selector_background_;
    ColourWidget *colour_selector_foreground_;
    ColourWidget *colour_selector_trim_;

    // Jersey designs
    QComboBox *jersey_selector_foreground_;
    QComboBox *jersey_selector_trim_;
    QComboBox *jersey_selector_preset_;
    QRadioButton *use_jersey_layer_images_;
    QRadioButton *use_jersey_preset_image_;

    // Preview
    Jersey generateJersey() const;
    QLabel *jersey_image_preview_;

    // Settings widget
    QWidget *createSettingsWidget();
    QGroupBox *createColourSelectionGroup();
    QGroupBox *createLayerSelectionGroup();
    QGroupBox *createTextDetailsGroup();

    // Text settings
    QLineEdit *jersey_name_input_;
    QSpinBox *jersey_number_input_;

private slots:
    void onJerseyImageSettingChange(const bool use_layers);
    void onPreview();
    void onSave();
};

#endif // PREVIEW_WIDGET_H
