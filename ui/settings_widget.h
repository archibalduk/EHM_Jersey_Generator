#ifndef SETTINGS_WIDGET_H
#define SETTINGS_WIDGET_H

// Application headers
#include "../dimensions.h"
#include "../jersey/font_server.h"

// Qt headers
class QCheckBox;
class QComboBox;
class QGroupBox;
class QSpinBox;
#include <QWidget>

// --- Settings widget --- //
class SettingsWidget : public QWidget
{
public:
    SettingsWidget(QWidget *parent = nullptr);

private:
    // Font
    QGroupBox *createGeneralSettingsGroup();
    QComboBox *font_selector_;
    FontServer font_server_;

    // Text settings
    QGroupBox *createTextSettingsGroup(const QString &settings_group_title,
                                       const QString &registry_settings_name_prefix);

    // Widgets
    QCheckBox *createCheckBox(const QString &registry_settings_name,
                              QWidget *parent,
                              const bool default_value = false);
    QSpinBox *createSpinBox(const QString &registry_settings_name,
                            QWidget *parent,
                            const qint32 minimum_value,
                            const qint32 maximum_value);

    enum ENUM_RANGES {
        MINIMUM_HORIZONTAL_POSITION_OFFSET = 0 - Dimensions::JerseyImageWidth,
        MAXIMUM_HORIZONTAL_POSITION_OFFSET = Dimensions::JerseyImageWidth,
        MINIMUM_VERTICAL_POSITION_OFFSET = 0 - Dimensions::JerseyImageHeight,
        MAXIMUM_VERTICAL_POSITION_OFFSET = Dimensions::JerseyImageHeight,
        MINIMUM_TEXT_SIZE_OFFSET = -100,
        MAXIMUM_TEXT_SIZE_OFFSET = 100
    };

private slots:
    // Set setting
    void setBoolSetting(const bool b);
    void setIntegerSetting(const qint32 i);
};

#endif // SETTINGS_WIDGET_H
