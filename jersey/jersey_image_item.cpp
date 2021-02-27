#include "jersey_image_item.h"

// Qt headers
#include <QFileInfo>

// --- Constructor --- //
JerseyImageItem::JerseyImageItem(const QString &file_name, const QString &display_name)
    : display_name_(std::move(display_name)), file_name_(std::move(file_name))
{
    if (display_name.isEmpty()) {
        const QFileInfo fi(file_name_);
        display_name_ = fi.baseName();
    }
}
