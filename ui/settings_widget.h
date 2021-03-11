#ifndef SETTINGS_WIDGET_H
#define SETTINGS_WIDGET_H

// Application headers
#include "../common/dimensions.h"
#include "../jersey/font_server.h"
#include "registry_settings_widget_server.h"

// Qt headers
class QComboBox;
class QGroupBox;

// --- Settings widget --- //
class SettingsWidget : public RegistrySettingsWidgetServer
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

    enum ENUM_RANGES {
        MINIMUM_JERSEY_NAME_UPSCALE_MULTIPLIER = 1,
        MAXIMUM_JERSEY_NAME_UPSCALE_MULTIPLIER = 10,
        MINIMUM_TEXT_CHARACTER_LIMIT = 0,
        MAXIMUM_TEXT_CHARACTER_LIMIT = 50,
        MINIMUM_HORIZONTAL_POSITION_OFFSET = 0 - Dimensions::JerseyImageWidth,
        MAXIMUM_HORIZONTAL_POSITION_OFFSET = Dimensions::JerseyImageWidth,
        MINIMUM_IMAGE_QUALITY = -1,
        MAXIMUM_IMAGE_QUALITY = 100,
        MINIMUM_VERTICAL_POSITION_OFFSET = 0 - Dimensions::JerseyImageHeight,
        MAXIMUM_VERTICAL_POSITION_OFFSET = Dimensions::JerseyImageHeight,
        MINIMUM_TEXT_SIZE_OFFSET = -100,
        MAXIMUM_TEXT_SIZE_OFFSET = 100,
        MINIMUM_TRIM_COLOUR_THRESHOLD = 0,
        MAXIMUM_TRIM_COLOUR_THRESHOLD = 255
    };
};

#endif // SETTINGS_WIDGET_H
