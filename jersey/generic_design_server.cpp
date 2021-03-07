#include "generic_design_server.h"

// Application headers
#include "../common/text.h"
#include "jersey_image_server.h"

/* =============================== */
/*      Generic Design Server      */
/* =============================== */

// --- Constructor --- //
GenericDesignServer::GenericDesignServer() {}

/* ================== */
/*      File i/o      */
/* ================== */

// --- Load the spreadsheet and process the designs --- //
void GenericDesignServer::init(const QString &file_path)
{
    QXlsx::Document spreadsheet(file_path);
    if (!spreadsheet.load())
        return;

    // Spreadsheet dimensions
    const auto row_count{spreadsheet.dimension().lastRow() + 1};
    const auto first_row{1}; // Row 0 = header row

    for (qint32 i = first_row; i < row_count; ++i)
        readSpreadsheetRow(spreadsheet, i);
}

// --- Read a spreadsheet row and return a design item --- //
DesignItem GenericDesignServer::readJerseyNames(QXlsx::Document &spreadsheet,
                                                const qint32 row,
                                                const qint32 foreground_layer_name_column,
                                                const qint32 trim_layer_name_column) const
{
    const Text foreground_layer_name{spreadsheet.read(row, foreground_layer_name_column).toString()};
    const Text trim_layer_name{spreadsheet.read(row, trim_layer_name_column).toString()};

    const auto foreground_layer_id{
        JerseyImageServer::foregroundLayers().find(foreground_layer_name.simpleStringLowerCase())};
    const auto trim_layer_id{
        JerseyImageServer::trimLayers().find(trim_layer_name.simpleStringLowerCase())};

    return DesignItem(foreground_layer_id, trim_layer_id);
}

// --- Read a row of a spreadsheet --- //
void GenericDesignServer::readSpreadsheetRow(QXlsx::Document &spreadsheet, const qint32 row)
{
    const auto generic_design_item{readJerseyNames(spreadsheet,
                                                   row,
                                                   FOREGROUND_GENERIC_JERSEY_LAYER_NAME,
                                                   TRIM_GENERIC_JERSEY_LAYER_NAME)};

    if (generic_design_item.isValid())
        jersey_designs_.push_back(generic_design_item);
}

/* ================== */
/*      Get Data      */
/* ================== */

// --- Find foreground jersey layer id from the percentage position in the jersey_designs_ vector --- //
qint32 GenericDesignServer::findForeground(const qreal &percentage) const
{
    const auto final_element_position{size() - 1};
    const auto element_position{round(percentage * static_cast<qreal>(final_element_position))};

    return foreground(element_position);
}

// --- Find trim jersey layer id from the percentage position in the jersey_designs_ vector --- //
qint32 GenericDesignServer::findTrim(const qreal &percentage) const
{
    const auto final_element_position{size() - 1};
    const auto element_position{round(percentage * static_cast<qreal>(final_element_position))};

    return trim(element_position);
}

// --- Get foreground layer id from the jersey_designs_ vector --- //
qint32 GenericDesignServer::foreground(qint32 generic_jersey_layer_element_pos) const
{
    if (generic_jersey_layer_element_pos < 0 || generic_jersey_layer_element_pos >= size())
        return 0;

    return jersey_designs_[generic_jersey_layer_element_pos].foreground();
}

// --- Get trim layer id from the jersey_designs_ vector --- //
qint32 GenericDesignServer::trim(qint32 generic_jersey_layer_element_pos) const
{
    if (generic_jersey_layer_element_pos < 0 || generic_jersey_layer_element_pos >= size())
        return 0;

    return jersey_designs_[generic_jersey_layer_element_pos].trim();
}
