#include "settings_widget.h"

// Application headers
#include "../dimensions.h"

// Qt headers
#include <QCheckBox>
#include <QComboBox>
#include <QFormLayout>
#include <QGroupBox>
#include <QLabel>
#include <QSettings>
#include <QSpinBox>
#include <QVBoxLayout>

// --- Constructor --- //
SettingsWidget::SettingsWidget(QWidget *parent) : QWidget(parent)
{
    setMinimumWidth(Dimensions::WidgetPanelWidth);

    // Layout and groups
    auto layout{new QVBoxLayout(this)};
    layout->addWidget(createGeneralSettingsGroup());
    layout->addWidget(createTextSettingsGroup(tr("Player Name"), "name"));
    layout->addWidget(createTextSettingsGroup(tr("Jersey Number"), "number"));
}

// --- Create general settings --- //
QGroupBox *SettingsWidget::createGeneralSettingsGroup()
{
    auto group{new QGroupBox(tr("Jersey Settings"), this)};

    font_selector_ = new QComboBox(group);
    font_server_.setComboBox(font_selector_);

    auto two_tone_layer{createCheckBox("two_tone_layer", group)};
    auto upper_case_name_text{createCheckBox("upper_case_name_text", group, true)};

    auto layout{new QFormLayout(group)};
    layout->addRow(tr("Font:"), font_selector_);
    layout->addRow(tr("Use two tone effect:"), two_tone_layer);
    layout->addRow(tr("Use upper case text:"), upper_case_name_text);

    return group;
}

// --- Create the text settings --- //
QGroupBox *SettingsWidget::createTextSettingsGroup(const QString &settings_group_title,
                                                   const QString &registry_settings_name_prefix)
{
    auto group{new QGroupBox(QString("%1 Settings").arg(settings_group_title), this)};

    auto horizontal_position_offset_{
        createSpinBox(QString("%1_text_horizontal_offset").arg(registry_settings_name_prefix),
                      group,
                      MINIMUM_HORIZONTAL_POSITION_OFFSET,
                      MAXIMUM_HORIZONTAL_POSITION_OFFSET)};

    auto vertical_position_offset_{
        createSpinBox(QString("%1_text_vertical_offset").arg(registry_settings_name_prefix),
                      group,
                      MINIMUM_VERTICAL_POSITION_OFFSET,
                      MAXIMUM_VERTICAL_POSITION_OFFSET)};

    auto text_size_offset_{
        createSpinBox(QString("%1_text_size_offset").arg(registry_settings_name_prefix),
                      group,
                      MINIMUM_TEXT_SIZE_OFFSET,
                      MAXIMUM_TEXT_SIZE_OFFSET)};

    auto layout{new QFormLayout(group)};
    layout->addRow(tr("Adjust horizontal position:"), horizontal_position_offset_);
    layout->addRow(tr("Adjust vertical position:"), vertical_position_offset_);
    layout->addRow(tr("Adjust text size:"), text_size_offset_);

    return group;
}

// --- Create a checkbox --- //
QCheckBox *SettingsWidget::createCheckBox(const QString &registry_settings_name,
                                          QWidget *parent,
                                          const bool default_value)
{
    auto check_box{new QCheckBox(parent)};
    check_box->setObjectName(registry_settings_name);
    QSettings settings;
    check_box->setChecked(settings.value(check_box->objectName(), default_value).toBool());
    QObject::connect(check_box, &QCheckBox::toggled, this, &SettingsWidget::setBoolSetting);

    return check_box;
}

// --- Create a spinbox --- //
QSpinBox *SettingsWidget::createSpinBox(const QString &registry_settings_name,
                                        QWidget *parent,
                                        const qint32 minimum_value,
                                        const qint32 maximum_value)
{
    auto spin_box{new QSpinBox(parent)};
    spin_box->setObjectName(registry_settings_name);
    spin_box->setRange(minimum_value, maximum_value);
    QSettings settings;
    spin_box->setValue(settings.value(spin_box->objectName(), 0).toInt());
    QObject::connect(spin_box, &QSpinBox::valueChanged, this, &SettingsWidget::setIntegerSetting);

    return spin_box;
}

// --- Set a setting (bool) --- //
void SettingsWidget::setBoolSetting(const bool b)
{
    QSettings settings;
    settings.setValue(QObject::sender()->objectName(), b);
}

// --- Set a setting (integer) --- //
void SettingsWidget::setIntegerSetting(const qint32 i)
{
    QSettings settings;
    settings.setValue(QObject::sender()->objectName(), i);
}
