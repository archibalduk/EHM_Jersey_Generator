#ifndef GENERIC_DESIGN_SERVER_H
#define GENERIC_DESIGN_SERVER_H

// Application headers
#include "design_item.h"
#include "xlsxdocument.h"

// Qt headers
#include <QString>

// --- Generic jersey design server --- //
class GenericDesignServer
{
public:
    GenericDesignServer();

    // File i/o
    void init(const QString &file_path);

    // Get data
    qint32 findForeground(const qreal &percentage) const;
    qint32 findTrim(const qreal &percentage) const;
    qint32 foreground(qint32 generic_jersey_layer_element_pos) const;
    inline qint32 size() const { return static_cast<qint32>(jersey_designs_.size()); }
    qint32 trim(qint32 generic_jersey_layer_element_pos) const;

protected:
    // Jersey designs (aka layer combinations)
    std::vector<DesignItem> jersey_designs_;

    // File i/o
    DesignItem readJerseyNames(QXlsx::Document &spreadsheet,
                               const qint32 row,
                               const qint32 foreground_layer_name_column,
                               const qint32 trim_layer_name_column) const;

private:
    // File i/o
    virtual void readSpreadsheetRow(QXlsx::Document &spreadsheet, const qint32 row);

    enum ENUM_GENERIC_SPREADSHEET_COLUMNS {
        FOREGROUND_GENERIC_JERSEY_LAYER_NAME = 1,
        TRIM_GENERIC_JERSEY_LAYER_NAME
    };
};

#endif // GENERIC_DESIGN_SERVER_H
