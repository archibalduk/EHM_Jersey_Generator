#ifndef BATCH_GENERATOR_WIDGET_H
#define BATCH_GENERATOR_WIDGET_H

// Qt headers
class QGroupBox;
class QLineEdit;
class QPushButton;
#include <QWidget>

// --- Batch generator widget --- //
class BatchGeneratorWidget : public QWidget
{
public:
    BatchGeneratorWidget(QWidget *parent = nullptr);
    ~BatchGeneratorWidget();

private:
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
    void onGenerate();
};

#endif // BATCH_GENERATOR_WIDGET_H
