#include "settings_widget.h"

// Application headers
#include "../common/dimensions.h"
#include "../jersey/jersey.h"

// Qt headers
#include <QCheckBox>
#include <QComboBox>
#include <QFormLayout>
#include <QGroupBox>
#include <QLabel>
#include <QSettings>
#include <QSpinBox>
#include <QVBoxLayout>

/* ========================= */
/*      Settings Widget      */
/* ========================= */

// --- Constructor --- //
SettingsWidget::SettingsWidget(QWidget *parent) : RegistrySettingsWidgetServer(parent)
{
    setMinimumWidth(Dimensions::WidgetPanelWidth);

    // Layout and groups
    auto layout{new QVBoxLayout(this)};
    layout->addWidget(createGeneralSettingsGroup());
    layout->addWidget(createTextSettingsGroup(tr("Player Name"), "name"));
    layout->addWidget(createTextSettingsGroup(tr("Jersey Number"), "number"));
}

/* ================= */
/*      Widgets      */
/* ================= */

// --- Create general settings --- //
QGroupBox *SettingsWidget::createGeneralSettingsGroup()
{
    auto group{new QGroupBox(tr("Jersey Settings"), this)};

    font_selector_ = new QComboBox(group);
    font_server_.setComboBox(font_selector_);

    auto image_quality{createSpinBox("image_quality",
                                     group,
                                     MINIMUM_IMAGE_QUALITY,
                                     MAXIMUM_IMAGE_QUALITY,
                                     MINIMUM_IMAGE_QUALITY)};

    auto trim_colour_threshold{createSpinBox("trim_colour_threshold",
                                             group,
                                             MINIMUM_TRIM_COLOUR_THRESHOLD,
                                             MAXIMUM_TRIM_COLOUR_THRESHOLD,
                                             Jersey::DEFAULT_TRIM_COLOUR_THRESHOLD)};

    auto accented_characters{createCheckBox("accented_characters", group)};
    auto two_tone_layer{createCheckBox("two_tone_layer", group)};
    auto upper_case_name_text{createCheckBox("upper_case_name_text", group, true)};

    auto layout{new QFormLayout(group)};
    layout->addRow(tr("Font:"), font_selector_);
    layout->addRow(tr("Allow accented characters:"), accented_characters);
    layout->addRow(tr("Two tone effect:"), two_tone_layer);
    layout->addRow(tr("Upper case text:"), upper_case_name_text);
    layout->addRow(tr("Image quality (0-100):"), image_quality);
    layout->addRow(tr("Trim colour threshold:"), trim_colour_threshold);

    return group;
}

// --- Create the text settings --- //
QGroupBox *SettingsWidget::createTextSettingsGroup(const QString &settings_group_title,
                                                   const QString &registry_settings_name_prefix)
{
    auto group{new QGroupBox(QString("%1 Settings").arg(settings_group_title), this)};

    auto horizontal_position_offset{
        createSpinBox(QString("%1_text_horizontal_offset").arg(registry_settings_name_prefix),
                      group,
                      MINIMUM_HORIZONTAL_POSITION_OFFSET,
                      MAXIMUM_HORIZONTAL_POSITION_OFFSET)};

    auto vertical_position_offset{
        createSpinBox(QString("%1_text_vertical_offset").arg(registry_settings_name_prefix),
                      group,
                      MINIMUM_VERTICAL_POSITION_OFFSET,
                      MAXIMUM_VERTICAL_POSITION_OFFSET)};

    auto text_size_offset{
        createSpinBox(QString("%1_text_size_offset").arg(registry_settings_name_prefix),
                      group,
                      MINIMUM_TEXT_SIZE_OFFSET,
                      MAXIMUM_TEXT_SIZE_OFFSET)};

    auto layout{new QFormLayout(group)};
    layout->addRow(tr("Adjust horizontal position:"), horizontal_position_offset);
    layout->addRow(tr("Adjust vertical position:"), vertical_position_offset);
    layout->addRow(tr("Adjust text size:"), text_size_offset);

    // Jersey text-specific settings
    if (registry_settings_name_prefix.compare("name", Qt::CaseInsensitive) == 0) {
        auto text_upscale_factor{
            createSpinBox(QString("%1_text_upscale_factor").arg(registry_settings_name_prefix),
                          group,
                          MINIMUM_JERSEY_NAME_UPSCALE_MULTIPLIER,
                          MAXIMUM_JERSEY_NAME_UPSCALE_MULTIPLIER,
                          Dimensions::DefaultJerseyNameUpscaleMultiplier)};
        layout->addRow(tr("Text quality (1-10):"), text_upscale_factor);

        auto text_character_limit{
            createSpinBox(QString("%1_text_character_limit").arg(registry_settings_name_prefix),
                          group,
                          MINIMUM_TEXT_CHARACTER_LIMIT,
                          MAXIMUM_TEXT_CHARACTER_LIMIT,
                          Jersey::DEFAULT_JERSEY_TEXT_CHARACTER_LIMIT)};
        layout->addRow(tr("Character limit before resize:"), text_character_limit);
    }

    return group;
}
