#ifndef PREVIEW_WIDGET_H
#define PREVIEW_WIDGET_H

// Application headers
class ColourWidget;
#include "../jersey/jersey.h"

// Qt headers
class QLabel;
class QLineEdit;
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

    // Preview
    Jersey generateJersey() const;
    QLabel *jersey_image_preview_;

    // Settings widget
    QWidget *createSettingsWidget();

    // Text settings
    QLineEdit *jersey_name_input_;
    QSpinBox *jersey_number_input_;

private slots:
    void onPreview();
    void onSave();
};

#endif // PREVIEW_WIDGET_H
