#ifndef REGISTRY_SETTINGS_WIDGET_SERVER_H
#define REGISTRY_SETTINGS_WIDGET_SERVER_H

// Qt headers
class QCheckBox;
class QDoubleSpinBox;
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
    QDoubleSpinBox *createDoubleSpinBox(const QString &registry_settings_name,
                                        QWidget *parent,
                                        const qreal minimum_value,
                                        const qreal maximum_value,
                                        const qreal default_value = 0.0);
    QSpinBox *createSpinBox(const QString &registry_settings_name,
                            QWidget *parent,
                            const qint32 minimum_value,
                            const qint32 maximum_value,
                            const qint32 default_value = 0);

private:
    enum ENUM_FLAGS { DECIMAL_PRECISION = 3 };
private slots:
    // Set setting
    void setBoolSetting(const bool b);
    void setDoubleSetting(const qreal i);
    void setIntegerSetting(const qint32 i);
};

#endif // REGISTRY_SETTINGS_WIDGET_SERVER_H
