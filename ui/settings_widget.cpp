#include "settings_widget.h"

// Application headers
#include "../dimensions.h"

// Qt headers
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
    layout->addWidget(createFontGroup());
    layout->addWidget(createTextPositionGroup());
}

// --- Create ton settings --- //
QGroupBox *SettingsWidget::createFontGroup()
{
    auto group{new QGroupBox(tr("Font"), this)};

    font_selector_ = new QComboBox(group);
    font_server_.setComboBox(font_selector_);

    auto label{
        new QLabel(tr("Select the jersey text font. Any font marked as 'no accents' will "
                      "do not support accents and will automatically remove accents from any "
                      "characters."),
                   group)};
    label->setWordWrap(true);

    auto layout{new QFormLayout(group)};
    layout->addRow(tr("Font:"), font_selector_);
    layout->addRow(label);

    return group;
}

// --- Create the text position settings --- //
QGroupBox *SettingsWidget::createTextPositionGroup()
{
    auto group{new QGroupBox(tr("Text positioning"), this)};

    QSettings settings;

    name_text_vertical_position_offset_ = new QSpinBox(group);
    name_text_vertical_position_offset_->setRange(MINIMUM_VERTICAL_POSITION_OFFSET,
                                                  MAXIMUM_VERTICAL_POSITION_OFFSET);
    name_text_vertical_position_offset_->setValue(
        settings.value("name_text_vertical_offset", 0).toInt());
    QObject::connect(name_text_vertical_position_offset_,
                     &QSpinBox::valueChanged,
                     this,
                     &SettingsWidget::setNameTextVerticalPositionOffset);

    number_text_vertical_position_offset_ = new QSpinBox(group);
    number_text_vertical_position_offset_->setRange(MINIMUM_VERTICAL_POSITION_OFFSET,
                                                    MAXIMUM_VERTICAL_POSITION_OFFSET);
    number_text_vertical_position_offset_->setValue(
        settings.value("number_text_vertical_offset", 0).toInt());
    QObject::connect(number_text_vertical_position_offset_,
                     &QSpinBox::valueChanged,
                     this,
                     &SettingsWidget::setNumberTextVerticalPositionOffset);

    auto label{new QLabel(tr("Adjust the vertical position (in pixels) of the jersey name and "
                             "number. A negative number moves the text upward and a positive "
                             "number moves the text downward."),
                          group)};
    label->setWordWrap(true);

    auto layout{new QFormLayout(group)};
    layout->addRow(tr("Name vertical offset:"), name_text_vertical_position_offset_);
    layout->addRow(tr("Number vertical offset:"), number_text_vertical_position_offset_);
    layout->addRow(label);

    return group;
}

// --- Set name text vertical position offset --- //
void SettingsWidget::setNameTextVerticalPositionOffset(const qint32 i)
{
    QSettings settings;
    settings.setValue("name_text_vertical_offset", i);
}

// --- Set number text vertical position offset --- //
void SettingsWidget::setNumberTextVerticalPositionOffset(const qint32 i)
{
    QSettings settings;
    settings.setValue("number_text_vertical_offset", i);
}
