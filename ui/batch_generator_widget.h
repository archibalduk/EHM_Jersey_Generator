#ifndef BATCH_GENERATOR_WIDGET_H
#define BATCH_GENERATOR_WIDGET_H

// Application headers
#include "registry_settings_widget_server.h"

// Qt headers
class QComboBox;
class QDoubleSpinBox;
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
    QCheckBox *use_preset_images_;
    QCheckBox *use_team_layer_designs_;

    // Generic design settings
    QGroupBox *createGenericDesignSettingsWidget();
    QDoubleSpinBox *average_character_random_seed_;
    QComboBox *generic_design_method_selector_;

    // Generic design selection
    QComboBox *jersey_selector_foreground_;
    QComboBox *jersey_selector_trim_;

    // File paths
    QGroupBox *createFilePathWidget();
    QWidget *createFilePathItem(QLineEdit *line_edit,
                                QPushButton *button,
                                const QString &object_name);
    QLineEdit *input_file_path_;
    QLineEdit *generic_jersey_design_file_path_;
    QLineEdit *team_jersey_design_file_path_;
    QLineEdit *output_folder_path_;

    // Initialisation
    void init();

    enum ENUM_DIMENSIONS { BUTTON_WIDTH = 100 };

private slots:
    void onGenerate() const;
    void setGenericJerseyDesign(const qint32 index);
};

#endif // BATCH_GENERATOR_WIDGET_H
