#ifndef FONT_ITEM_H
#define FONT_ITEM_H

// Qt headers
#include <QString>

// --- Font item data --- //
class FontItem
{
public:
    FontItem(const QString &file_name, const bool allow_accented_characters);

    // Get data
    inline bool allowAccentedCharacters() const { return allow_accented_characters_; }
    inline QString displayName() const { return font_family_; }
    inline QString fileName() const { return font_file_name_; }
    inline QString fontFamily() const { return font_family_; }

private:
    QString font_family_;
    const QString font_file_name_;
    qint32 font_id_{0};
    const bool allow_accented_characters_;
};

#endif // FONT_ITEM_H
