#ifndef REGISTRY_SETTINGS_WIDGET_SERVER_H
#define REGISTRY_SETTINGS_WIDGET_SERVER_H

// Qt headers
class QCheckBox;
class QSpinBox;
class QString;
#include <QWidget>

// --- Regstry settings widget server --- //
class RegistrySettingsWidgetServer : public QWidget
{
public:
    RegistrySettingsWidgetServer(QWidget *parent = nullptr);

    // Widgets
    QCheckBox *createCheckBox(const QString &registry_settings_name,
                              QWidget *parent,
                              const bool default_value = false);
    QSpinBox *createSpinBox(const QString &registry_settings_name,
                            QWidget *parent,
                            const qint32 minimum_value,
                            const qint32 maximum_value,
                            const qint32 default_value = 0);

private slots:
    // Set setting
    void setBoolSetting(const bool b);
    void setIntegerSetting(const qint32 i);
};

#endif // REGISTRY_SETTINGS_WIDGET_SERVER_H
