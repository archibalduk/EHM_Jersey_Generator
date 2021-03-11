#include "batch_generator_widget.h"

// Application headers
#include "../batch_generator/batch_generator.h"
#include "../common/text.h"
#include "../jersey/jersey.h"

// Qt headers
#include <QCheckBox>
#include <QComboBox>
#include <QDir>
#include <QFileDialog>
#include <QFormLayout>
#include <QGridLayout>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QSettings>
#include <QSpinBox>

/* ================================ */
/*      Batch Generator Widget      */
/* ================================ */

// --- Constructor --- //
BatchGeneratorWidget::BatchGeneratorWidget(QWidget *parent) : RegistrySettingsWidgetServer(parent)
{
    // Layout
    auto layout{new QGridLayout(this)};
    layout->addWidget(createDesignSelectionWidget(), 0, 0);
    layout->addWidget(createGenericDesignSettingsWidget(), 0, 1);
    layout->addWidget(createFilePathWidget(), 1, 0, 1, 2);

    auto generate_button{new QPushButton(tr("Generate Jerseys"), this)};
    QObject::connect(generate_button,
                     &QPushButton::clicked,
                     this,
                     &BatchGeneratorWidget::onGenerate);
    layout->addWidget(generate_button, 2, 0, 1, 2);

    // Initialise with default settings
    init();
}

// --- Destructor --- //
BatchGeneratorWidget::~BatchGeneratorWidget()
{
    QSettings settings;
    settings.setValue(input_file_path_->objectName(), input_file_path_->text());
    settings.setValue(generic_jersey_design_file_path_->objectName(),
                      generic_jersey_design_file_path_->text());
    settings.setValue(output_folder_path_->objectName(), output_folder_path_->text());
    settings.setValue(generic_design_method_selector_->objectName(),
                      generic_design_method_selector_->currentIndex());
}

/* ================================== */
/*      Design Selection Widgets      */
/* ================================== */

// --- Create the file path widget --- //
QGroupBox *BatchGeneratorWidget::createDesignSelectionWidget()
{
    auto group{new QGroupBox(tr("Jersey Design Settings"), this)};

    use_preset_images_ = createCheckBox("use_preset_images", group, true);
    use_team_layer_designs_ = createCheckBox("use_team_layer_designs", group, true);

    auto layout{new QFormLayout(group)};
    layout->addRow(tr("Use preset jersey images:"), use_preset_images_);
    layout->addRow(tr("Use team layer designs:"), use_team_layer_designs_);

    return group;
}

/* =========================== */
/*      File Path Widgets      */
/* =========================== */

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
    layout->addRow(tr("Generic jersey layer designs:"), generic_jersey_design_file_path_widget);
    layout->addRow(tr("Team jersey layer designs:"), team_jersey_design_file_path_widget);
    layout->addRow(tr("Input file:"), input_file_path_widget);
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

/* =================== */
/*      Generator      */
/* =================== */

// --- Import and generate the jerseys --- //
void BatchGeneratorWidget::onGenerate() const
{
    BatchGenerator generator(input_file_path_->text(),
                             output_folder_path_->text(),
                             generic_jersey_design_file_path_->text(),
                             team_jersey_design_file_path_->text());

    Jersey::setDefaultImages(jersey_selector_foreground_->currentIndex(),
                             jersey_selector_trim_->currentIndex());

    generator.generate();
}

/* ================================= */
/*      Generic Design Settings      */
/* ================================= */

// --- Create the generic design settings widget --- //
QGroupBox *BatchGeneratorWidget::createGenericDesignSettingsWidget()
{
    auto group{new QGroupBox(tr("Generic Design Settings"), this)};

    generic_design_method_selector_ = new QComboBox(this);
    generic_design_method_selector_->setObjectName("generic_design_method");
    generic_design_method_selector_->insertItem(Jersey::FIXED_LAYERS, tr("Use a single design"));
    generic_design_method_selector_->insertItem(Jersey::GENERIC_LAYERS_BY_CLUB_NAME,
                                                tr("Use generic_jerseys.xlsx by team"));
    generic_design_method_selector_->insertItem(Jersey::PURE_RANDOM,
                                                tr("Use random designs by player"));
    generic_design_method_selector_->insertItem(Jersey::RANDOM_LAYERS_BY_CLUB_NAME,
                                                tr("Use random designs by team"));
    QObject::connect(generic_design_method_selector_,
                     &QComboBox::currentIndexChanged,
                     this,
                     &BatchGeneratorWidget::setGenericJerseyDesign);

    average_character_random_seed_ = {createDoubleSpinBox("average_character_random_seed",
                                                          group,
                                                          Text::MINIMUM_RANDOM_SEED,
                                                          Text::MAXIMUM_RANDOM_SEED)};

    jersey_selector_foreground_ = new QComboBox(group);
    JerseyImageServer::foregroundLayers().setComboBox(jersey_selector_foreground_);

    jersey_selector_trim_ = new QComboBox(group);
    JerseyImageServer::trimLayers().setComboBox(jersey_selector_trim_);

    auto layout{new QFormLayout(group)};
    layout->addRow(tr("Generic design method:"), generic_design_method_selector_);
    layout->addRow(tr("Random seed:"), average_character_random_seed_);
    layout->addRow(tr("Foreground layer:"), jersey_selector_foreground_);
    layout->addRow(tr("Trim layer:"), jersey_selector_trim_);

    return group;
}

// --- Set the generic jersey design settings --- //
void BatchGeneratorWidget::setGenericJerseyDesign(const qint32 index)
{
    // Save the setting
    QSettings settings;
    settings.setValue(generic_design_method_selector_->objectName(),
                      generic_design_method_selector_->currentIndex());

    // Update the widgets
    average_character_random_seed_->setEnabled(index == Jersey::GENERIC_LAYERS_BY_CLUB_NAME
                                               || index == Jersey::RANDOM_LAYERS_BY_CLUB_NAME);
    generic_jersey_design_file_path_->setEnabled(index == Jersey::GENERIC_LAYERS_BY_CLUB_NAME);
    jersey_selector_foreground_->setEnabled(index == Jersey::FIXED_LAYERS);
    jersey_selector_trim_->setEnabled(index == Jersey::FIXED_LAYERS);
}

/* ======================== */
/*      Initialisation      */
/* ======================== */

// --- Initialise the connections/settings --- //
void BatchGeneratorWidget::init()
{
    QSettings settings;

    // Generic design method
    generic_design_method_selector_->setCurrentIndex(
        settings
            .value(generic_design_method_selector_->objectName(),
                   Jersey::GENERIC_LAYERS_BY_CLUB_NAME)
            .toInt());
    setGenericJerseyDesign(generic_design_method_selector_->currentIndex());

    // Design selection
    QObject::connect(use_team_layer_designs_,
                     &QCheckBox::toggled,
                     team_jersey_design_file_path_,
                     &QLineEdit::setEnabled);
    team_jersey_design_file_path_->setEnabled(use_team_layer_designs_->isChecked());
}
