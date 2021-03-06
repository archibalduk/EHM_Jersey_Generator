#include "font_item.h"

// Application headers
#include <QFontDatabase>

// --- Constructor --- //
FontItem::FontItem(const QString &file_name, const bool allow_accented_characters)
    : font_file_name_(file_name), allow_accented_characters_(allow_accented_characters)
{
    font_id_ = QFontDatabase::addApplicationFont(font_file_name_);

    const auto font_families{QFontDatabase::applicationFontFamilies(font_id_)};

    if (font_families.size() > 0)
        font_family_ = QFontDatabase::applicationFontFamilies(font_id_).at(0);
}
