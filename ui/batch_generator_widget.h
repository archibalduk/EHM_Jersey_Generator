#ifndef BATCH_GENERATOR_WIDGET_H
#define BATCH_GENERATOR_WIDGET_H

// Application headers
#include "registry_settings_widget_server.h"

// Qt headers
class QGroupBox;
class QLineEdit;
class QPushButton;

// --- Batch generator widget --- //
class BatchGeneratorWidget : public RegistrySettingsWidgetServer
{
public:
    BatchGeneratorWidget(QWidget *parent = nullptr);
    ~BatchGeneratorWidget();

private:
    // Design selection
    QGroupBox *createDesignSelectionWidget();

    // File paths
    QGroupBox *createFilePathWidget();
    QWidget *createFilePathItem(QLineEdit *line_edit,
                                QPushButton *button,
                                const QString &object_name);
    QLineEdit *input_file_path_;
    QLineEdit *generic_jersey_design_file_path_;
    QLineEdit *team_jersey_design_file_path_;
    QLineEdit *output_folder_path_;

    enum ENUM_DIMENSIONS { BUTTON_WIDTH = 100 };

private slots:
    void onGenerate() const;
};

#endif // BATCH_GENERATOR_WIDGET_H
