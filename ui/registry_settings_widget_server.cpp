#include "registry_settings_widget_server.h"

// Qt headers
#include <QCheckBox>
#include <QObject>
#include <QSettings>
#include <QSpinBox>

/* ========================================= */
/*      Registry Settings Widget Server      */
/* ========================================= */

// --- Constructor --- //
RegistrySettingsWidgetServer::RegistrySettingsWidgetServer(QWidget *parent) : QWidget(parent) {}

/* ================== */
/*      Set Data      */
/* ================== */

// --- Set a setting (bool) --- //
void RegistrySettingsWidgetServer::setBoolSetting(const bool b)
{
    QSettings settings;
    settings.setValue(QObject::sender()->objectName(), b);
}

// --- Set a setting (integer) --- //
void RegistrySettingsWidgetServer::setIntegerSetting(const qint32 i)
{
    QSettings settings;
    settings.setValue(QObject::sender()->objectName(), i);
}

/* =========================== */
/*      Widget Generation      */
/* =========================== */

// --- Create a checkbox --- //
QCheckBox *RegistrySettingsWidgetServer::createCheckBox(const QString &registry_settings_name,
                                                        QWidget *parent,
                                                        const bool default_value)
{
    auto check_box{new QCheckBox(parent)};
    check_box->setObjectName(registry_settings_name);
    QSettings settings;
    check_box->setChecked(settings.value(check_box->objectName(), default_value).toBool());
    QObject::connect(check_box,
                     &QCheckBox::toggled,
                     this,
                     &RegistrySettingsWidgetServer::setBoolSetting);

    return check_box;
}

// --- Create a spinbox --- //
QSpinBox *RegistrySettingsWidgetServer::createSpinBox(const QString &registry_settings_name,
                                                      QWidget *parent,
                                                      const qint32 minimum_value,
                                                      const qint32 maximum_value,
                                                      const qint32 default_value)
{
    auto spin_box{new QSpinBox(parent)};
    spin_box->setObjectName(registry_settings_name);
    spin_box->setRange(minimum_value, maximum_value);
    QSettings settings;
    spin_box->setValue(settings.value(spin_box->objectName(), default_value).toInt());
    QObject::connect(spin_box,
                     &QSpinBox::valueChanged,
                     this,
                     &RegistrySettingsWidgetServer::setIntegerSetting);

    return spin_box;
}
