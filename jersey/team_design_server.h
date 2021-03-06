#ifndef TEAM_DESIGN_SERVER_H
#define TEAM_DESIGN_SERVER_H

// Application headers
#include "generic_design_server.h"
class Text;

// Qt headers
#include <QHash>

// --- Team jersey design server --- //
class TeamDesignServer : public GenericDesignServer
{
public:
    TeamDesignServer();

    // Get data
    qint32 findTeam(const Text &team_name) const;

private:
    // Team name list
    QHash<QString, qint32> team_names_;

    // File i/o
    void readSpreadsheetRow(QXlsx::Document &spreadsheet, const qint32 row) override;

    enum ENUM_TEAM_SPREADSHEET_COLUMNS {
        TEAM_NAME = 1,
        FOREGROUND_TEAM_JERSEY_LAYER_NAME,
        TRIM_TEAM_JERSEY_LAYER_NAME
    };
};

#endif // TEAM_DESIGN_SERVER_H
