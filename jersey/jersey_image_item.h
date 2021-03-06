#ifndef JERSEY_IMAGE_ITEM_H
#define JERSEY_IMAGE_ITEM_H

// Qt headers
#include <QString>

// --- Jersey image file item data --- //
class JerseyImageItem
{
public:
    JerseyImageItem(const QString &file_name, const QString &display_name = QString());

    // Get data
    inline QString displayName() const { return display_name_; }
    inline QString fileName() const { return file_name_; }
    QString simpleName() const;

private:
    QString display_name_;
    const QString file_name_;
};

#endif // JERSEY_IMAGE_ITEM_H
