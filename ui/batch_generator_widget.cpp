#include "batch_generator_widget.h"

// Application headers
#include "../batch_generator/batch_generator.h"

// Qt headers
#include <QDir>
#include <QFileDialog>
#include <QFormLayout>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QSettings>
#include <QVBoxLayout>

// --- Constructor --- //
BatchGeneratorWidget::BatchGeneratorWidget(QWidget *parent) : QWidget(parent)
{
    // Layout
    auto layout{new QVBoxLayout(this)};
    layout->addWidget(createFilePathWidget());

    auto generate_button{new QPushButton(tr("Generate Jerseys"), this)};
    QObject::connect(generate_button,
                     &QPushButton::clicked,
                     this,
                     &BatchGeneratorWidget::onGenerate);
    layout->addWidget(generate_button);
}

// --- Destructor --- //
BatchGeneratorWidget::~BatchGeneratorWidget()
{
    QSettings settings;
    settings.setValue(input_file_path_->objectName(), input_file_path_->text());
    settings.setValue(generic_jersey_design_file_path_->objectName(),
                      generic_jersey_design_file_path_->text());
    settings.setValue(output_folder_path_->objectName(), output_folder_path_->text());
}

// --- Create the file path widget --- //
QGroupBox *BatchGeneratorWidget::createFilePathWidget()
{
    auto group{new QGroupBox(tr("File Locations"), this)};

    // Generic jersey design file
    generic_jersey_design_file_path_ = new QLineEdit(group);
    auto generic_jersey_design_file_path_button = new QPushButton(tr("Select"),
                                                                  generic_jersey_design_file_path_);
    const auto generic_jersey_design_file_path_widget
        = createFilePathItem(generic_jersey_design_file_path_,
                             generic_jersey_design_file_path_button,
                             "generic_jersey_designs_file_path");

    if (generic_jersey_design_file_path_->text().isEmpty()) // Default jersey designs file path
        generic_jersey_design_file_path_->setText("generic_jerseys.xlsx");

    QObject::connect(generic_jersey_design_file_path_button, &QPushButton::clicked, [&]() {
        const auto initial_file_path{(generic_jersey_design_file_path_->text().isEmpty()
                                          ? QString() // Default to the installation folder
                                          : generic_jersey_design_file_path_->text())};

        const auto file_path{
            QFileDialog::getOpenFileName(this,
                                         tr("Open Generic Jersey Designs Spreadsheet"),
                                         initial_file_path,
                                         tr("Excel Spreadsheet (*.xlsx)"))};

        if (!file_path.isEmpty())
            generic_jersey_design_file_path_->setText(file_path);
    });

    // Specific team jersey design file
    team_jersey_design_file_path_ = new QLineEdit(group);
    auto team_jersey_design_file_path_button = new QPushButton(tr("Select"),
                                                               team_jersey_design_file_path_);
    const auto team_jersey_design_file_path_widget
        = createFilePathItem(team_jersey_design_file_path_,
                             team_jersey_design_file_path_button,
                             "team_jersey_designs_file_path");

    if (team_jersey_design_file_path_->text().isEmpty()) // Default jersey designs file path
        team_jersey_design_file_path_->setText("team_jerseys.xlsx");

    QObject::connect(team_jersey_design_file_path_button, &QPushButton::clicked, [&]() {
        const auto initial_file_path{(team_jersey_design_file_path_->text().isEmpty()
                                          ? QString() // Default to the installation folder
                                          : team_jersey_design_file_path_->text())};

        const auto file_path{
            QFileDialog::getOpenFileName(this,
                                         tr("Open Team Jersey Designs Spreadsheet"),
                                         initial_file_path,
                                         tr("Excel Spreadsheet (*.xlsx)"))};

        if (!file_path.isEmpty())
            team_jersey_design_file_path_->setText(file_path);
    });

    // Input file path
    input_file_path_ = new QLineEdit(group);
    auto input_file_path_button = new QPushButton(tr("Select"), input_file_path_);
    const auto input_file_path_widget = createFilePathItem(input_file_path_,
                                                           input_file_path_button,
                                                           "input_file_path");
    QObject::connect(input_file_path_button, &QPushButton::clicked, [&]() {
        const auto initial_file_path{
            (input_file_path_->text().isEmpty() ? QDir::homePath() : input_file_path_->text())};

        const auto file_path{QFileDialog::getOpenFileName(this,
                                                          tr("Open Player Data Spreadsheet"),
                                                          initial_file_path,
                                                          tr("Excel Spreadsheet (*.xlsx)"))};

        if (!file_path.isEmpty())
            input_file_path_->setText(file_path);
    });

    // Output file path
    output_folder_path_ = new QLineEdit(group);
    auto output_folder_path_button = new QPushButton(tr("Select"), output_folder_path_);
    const auto output_folder_path_widget = createFilePathItem(output_folder_path_,
                                                              output_folder_path_button,
                                                              "output_folder_path");
    QObject::connect(output_folder_path_button, &QPushButton::clicked, [&]() {
        const auto initial_file_path{(output_folder_path_->text().isEmpty()
                                          ? QDir::homePath()
                                          : output_folder_path_->text())};

        const auto folder_path{QFileDialog::getExistingDirectory(this,
                                                                 tr("Select Output Folder"),
                                                                 initial_file_path,
                                                                 QFileDialog::ShowDirsOnly)};

        if (!folder_path.isEmpty())
            output_folder_path_->setText(folder_path);
    });

    auto layout{new QFormLayout(group)};
    layout->addRow(tr("Generic jersey designs:"), generic_jersey_design_file_path_widget);
    layout->addRow(tr("Team jersey designs:"), team_jersey_design_file_path_widget);
    layout->addRow(tr("Player data:"), input_file_path_widget);
    layout->addRow(tr("Output folder:"), output_folder_path_widget);

    return group;
}

// --- Create a file path item with a line edit and push button --- //
QWidget *BatchGeneratorWidget::createFilePathItem(QLineEdit *line_edit,
                                                  QPushButton *button,
                                                  const QString &object_name)
{
    auto widget{new QWidget(this)};

    line_edit->setObjectName(object_name);
    button->setFixedWidth(BUTTON_WIDTH);

    QSettings settings;
    line_edit->setText(settings.value(line_edit->objectName(), QString()).toString());
    line_edit->setReadOnly(true);

    auto layout{new QHBoxLayout(widget)};
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(line_edit);
    layout->addWidget(button);

    return widget;
}

// --- Import and generate the jerseys --- //
void BatchGeneratorWidget::onGenerate()
{
    BatchGenerator generator(input_file_path_->text(),
                             output_folder_path_->text(),
                             generic_jersey_design_file_path_->text(),
                             team_jersey_design_file_path_->text());
    generator.generate();
}
