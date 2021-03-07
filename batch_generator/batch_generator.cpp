#include "batch_generator.h"

// Application headers
#include "../common/text.h"
#include "../jersey/jersey.h"
#include "xlsxdocument.h"

// Qt headers
#include <QApplication>
#include <QElapsedTimer>
#include <QFutureWatcher>
#include <QMessageBox>
#include <QProgressDialog>
#include <QSettings>
#include <QtConcurrent>

/* ========================= */
/*      Batch Generator      */
/* ========================= */

// --- Constructor --- //
BatchGenerator::BatchGenerator(const QString &input_file_path,
                               const QString &output_file_path,
                               const QString &generic_jersey_design_file_path,
                               const QString &team_jersey_design_file_path)
    : input_file_path_(input_file_path), output_folder_path_(output_file_path)
{
    QSettings settings;

    // Initialise the preset jersey server (use a blank server if presets are disabled)
    use_preset_images_ = settings.value("use_preset_images", true).toBool();
    if (use_preset_images_)
        preset_jersey_images_ = &JerseyImageServer::presetImages();
    else
        preset_jersey_images_ = new JerseyImageServer;

    // Process the generic and team jersey layer design files
    generic_jersey_designs_.init(generic_jersey_design_file_path);
    if (settings.value("use_team_layer_designs", true).toBool())
        team_jersey_designs_.init(team_jersey_design_file_path);

    // Set text random seed
    Text::setRandomSeed(
        settings.value("average_character_random_seed", Text::MINIMUM_RANDOM_SEED).toDouble());
}

// --- Destructor --- //
BatchGenerator::~BatchGenerator()
{
    if (!use_preset_images_)
        delete preset_jersey_images_;
}

/* =================== */
/*      Generator      */
/* =================== */

// --- Batch jersey image generation --- //
bool BatchGenerator::generate()
{
    // Timer and progress dialog
    QProgressDialog progress(QObject::tr("Processing spreadsheet"), QString(), 0, 1);
    progress.setWindowTitle(
        QObject::tr("Generating jerseys (using %L1 threads)").arg(QThread::idealThreadCount()));
    progress.show();
    QApplication::processEvents();

    QXlsx::Document spreadsheet(input_file_path_);
    if (!spreadsheet.load()) {
        QMessageBox::critical(nullptr,
                              QObject::tr("Spreadsheet error"),
                              QObject::tr("Unable to open %1.").arg(input_file_path_));
        progress.close();
        return false;
    }

    // Spreadsheet dimensions
    const auto first_row{2}; // Row 1 = header row
    const auto final_row{spreadsheet.dimension().lastRow() + 1};

    // Reset the progress dialog
    progress.setLabelText(QObject::tr("Processing %L1 rows of data").arg(final_row - first_row));
    progress.setMaximum(final_row);
    QApplication::processEvents();

    QElapsedTimer timer;
    timer.start();

    // Create a QFutureWatcher and connect signals and slots.
    QFutureWatcher<void> futureWatcher;
    QObject::connect(&futureWatcher,
                     &QFutureWatcher<void>::finished,
                     &progress,
                     &QProgressDialog::reset);
    QObject::connect(&progress,
                     &QProgressDialog::canceled,
                     &futureWatcher,
                     &QFutureWatcher<void>::cancel);
    QObject::connect(&futureWatcher,
                     &QFutureWatcher<void>::progressRangeChanged,
                     &progress,
                     &QProgressDialog::setRange);
    QObject::connect(&futureWatcher,
                     &QFutureWatcher<void>::progressValueChanged,
                     &progress,
                     &QProgressDialog::setValue);

    // Buffer the row references
    std::vector<qint32> buffer;
    for (qint32 i = first_row; i < final_row; ++i)
        buffer.push_back(i);

    // Jersey generation lambda
    std::function<bool(int &)> generate =
        [this, &spreadsheet, &progress, &futureWatcher](const int &i) {
            const auto player_surname{spreadsheet.read(i, PLAYER_SECOND_NAME).toString().trimmed()};
            if (player_surname.isEmpty())
                return true;

            Jersey jersey(player_surname, spreadsheet.read(i, JERSEY_NUMBER).toInt());
            jersey.setColours(spreadsheet.read(i, BACKGROUND_COLOUR).toString(),
                              spreadsheet.read(i, FOREGROUND_COLOUR).toString(),
                              spreadsheet.read(i, TRIM_COLOUR).toString());
            jersey.usePresetImage(true); // Use preset images where possible
            jersey.setImages(Text(spreadsheet.read(i, CLUB_NAME).toString()),
                             generic_jersey_designs_,
                             team_jersey_designs_,
                             *preset_jersey_images_);
            jersey.generate();

            const auto output_file_path{
                QString("%1/%2_%3_%4.png")
                    .arg(output_folder_path_,
                         spreadsheet.read(i, PLAYER_FIRST_NAME).toString().trimmed(),
                         player_surname,
                         spreadsheet.read(i, PLAYER_DATE_OF_BIRTH)
                             .toString()
                             .replace(".", "_")
                             .replace("/", "_"))};
            const auto result{jersey.save(output_file_path)};

            if (!result) {
                futureWatcher.cancel();
                progress.cancel();
                QMessageBox::critical(nullptr,
                                      QObject::tr("File write error (row %L1").arg(i),
                                      QObject::tr("Unable to save %1 to the disk.")
                                          .arg(output_file_path));
                return false;
            }

            return true;
        };

    // Start the computation
    futureWatcher.setFuture(QtConcurrent::map(buffer, generate));

    // Display the dialog and start the event loop.
    progress.exec();
    futureWatcher.waitForFinished();

    // Calculate the time elapsed once complete
    const qreal elapsed{qreal(timer.elapsed()) / 1'000}; // 1,000 milliseconds -> seconds
    progress.close();

    QMessageBox::information(nullptr,
                             QObject::tr("Complete"),
                             QObject::tr(
                                 "Batch generation of %L1 row(s) complete in %L2 second(s).")
                                 .arg(buffer.size())
                                 .arg(elapsed));

    return true;
}

// --- Increment the progress dialog
void BatchGenerator::updateProgressDialog(QProgressDialog &progress,
                                          const qint32 &total_rows_processed,
                                          QElapsedTimer &timer) const
{
    const qreal elapsed{static_cast<qreal>(timer.elapsed()) / 1'000};
    const qreal remaining{
        (((static_cast<qreal>(progress.maximum()) / static_cast<qreal>(total_rows_processed))
          * elapsed)
         - elapsed)
        / 1'000};

    progress.setLabelText(QString("%L1 seconds elapsed (%L2 seconds remaining)")
                              .arg(static_cast<qint32>(elapsed))
                              .arg(static_cast<qint32>(remaining)));
    progress.setValue(total_rows_processed);
    QApplication::processEvents();
}

/* OLD ITERATION CODE BELOW */
/*
    // Process each row
    quint32 total_jerseys_generated{0};
    for (qint32 i = first_row; i < row_count; ++i) {
        const auto player_surname{spreadsheet.read(i, PLAYER_SECOND_NAME).toString().trimmed()};
        if (player_surname.isEmpty())
            continue;

Jersey jersey(player_surname, spreadsheet.read(i, JERSEY_NUMBER).toInt());
jersey.setColours(spreadsheet.read(i, BACKGROUND_COLOUR).toString(),
                  spreadsheet.read(i, FOREGROUND_COLOUR).toString(),
                  spreadsheet.read(i, TRIM_COLOUR).toString());
jersey.usePresetImage(true); // Use preset images where possible
jersey.setImages(Text(spreadsheet.read(i, CLUB_NAME).toString()),
                 generic_jersey_designs_,
                 team_jersey_designs_);
jersey.generate();

const auto output_file_path{
    QString("%1/%2_%3_%4.png")
        .arg(output_folder_path_,
             spreadsheet.read(i, PLAYER_FIRST_NAME).toString().trimmed(),
             player_surname,
             spreadsheet.read(i, PLAYER_DATE_OF_BIRTH)
                 .toString()
                 .replace(".", "_")
                 .replace("/", "_"))};
const auto result{jersey.save(output_file_path)};

if (i % PROGRESS_BAR_REFRESH_RATE == 0)
    updateProgressDialog(progress, i, timer);

if (!result) {
    progress.cancel();
    
    QMessageBox::critical(
        nullptr,
        QObject::tr("File write error (row %L1 of %L2)").arg(i).arg(row_count - row_count),
        QObject::tr("Unable to save %1 to the disk.").arg(output_file_path));
    return false;
}

++total_jerseys_generated;
}

    const qreal elapsed{qreal(timer.elapsed()) / 1'000};
    progress.accept();
    progress.close();
    QApplication::processEvents();

*/
