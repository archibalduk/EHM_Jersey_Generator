#include "design_item.h"

// Application headers
#include "jersey_image_server.h"

/* ===================== */
/*      Design Item      */
/* ===================== */

// --- Constructor --- //
DesignItem::DesignItem(const qint32 &foreground_jersey_layer_id, const qint32 &trim_jersey_layer_id)
    : foreground_jersey_id_(foreground_jersey_layer_id), trim_jersey_id_(trim_jersey_layer_id)
{
    
}

/* ================== */
/*      Get Data      */
/* ================== */

// --- Check whether the item is valid --- //
bool DesignItem::isValid() const
{
    return (foreground_jersey_id_ != JerseyImageServer::NO_RESULT
            && trim_jersey_id_ != JerseyImageServer::NO_RESULT);
}

/* ================== */
/*      Set Data      */
/* ================== */

// --- Set jersey layer ids --- //
void DesignItem::set(const qint32 &foreground_jersey_layer_id, const qint32 &trim_jersey_layer_id)
{
    foreground_jersey_id_ = foreground_jersey_layer_id;
    trim_jersey_id_ = trim_jersey_layer_id;
}
