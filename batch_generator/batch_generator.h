#ifndef BATCH_GENERATOR_H
#define BATCH_GENERATOR_H

// Application headers
#include "../jersey/generic_design_server.h"
#include "../jersey/jersey_image_server.h"
#include "../jersey/team_design_server.h"

// Qt headers
class QElapsedTimer;
class QProgressDialog;
#include <QString>

// --- Batch jersey generator --- //
class BatchGenerator
{
public:
    BatchGenerator(const QString &input_file_path,
                   const QString &output_file_path,
                   const QString &generic_jersey_design_file_path,
                   const QString &team_jersey_design_file_path);
    ~BatchGenerator();

    // Image generation
    bool generate();

private:
    // File/folder paths
    const QString input_file_path_;
    const QString output_folder_path_;

    // Jersey designs
    GenericDesignServer generic_jersey_designs_;
    bool use_preset_images_{true};
    JerseyImageServer *preset_jersey_images_{nullptr};
    TeamDesignServer team_jersey_designs_;

    enum ENUM_SPREADSHEET_COLUMNS {
        PLAYER_FIRST_NAME = 1,
        PLAYER_SECOND_NAME,
        PLAYER_DATE_OF_BIRTH,
        CLUB_NAME,
        JERSEY_NUMBER,
        BACKGROUND_COLOUR,
        FOREGROUND_COLOUR,
        TRIM_COLOUR
    };

    enum ENUM_FLAGS { PROGRESS_BAR_REFRESH_RATE = 200 };
};

#endif // BATCH_GENERATOR_H
