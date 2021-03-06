#include "team_design_server.h"

// Application headers
#include "../common/text.h"
#include "jersey_image_server.h"
#include "xlsxdocument.h"

// --- Constructor --- //
TeamDesignServer::TeamDesignServer() {}

// --- Find the design element matching the selected team name --- //
qint32 TeamDesignServer::findTeam(const Text &team_name) const
{
    return team_names_.value(team_name.simpleStringLowerCase(), JerseyImageServer::NO_RESULT);
}

// --- Read a row of a spreadsheet --- //
void TeamDesignServer::readSpreadsheetRow(QXlsx::Document &spreadsheet, const qint32 row)
{
    const auto team_design_item{readJerseyNames(spreadsheet,
                                                row,
                                                FOREGROUND_TEAM_JERSEY_LAYER_NAME,
                                                TRIM_TEAM_JERSEY_LAYER_NAME)};

    if (team_design_item.isValid()) {
        const Text team_name{spreadsheet.read(row, TEAM_NAME).toString()};
        team_names_.insert(team_name.simpleStringLowerCase(), jersey_designs_.size());
        jersey_designs_.push_back(team_design_item);
    }
}
