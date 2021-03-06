#ifndef DESIGN_ITEM_H
#define DESIGN_ITEM_H

// Qt headers
#include <QtGlobal>

// --- Jersey layer design item --- //
class DesignItem
{
public:
    DesignItem(const qint32 &foreground_jersey_layer_id = 0, const qint32 &trim_jersey_layer_id = 0);

    // Get data
    inline qint32 foreground() const { return foreground_jersey_id_; }
    bool isValid() const;
    inline qint32 trim() const { return trim_jersey_id_; }

    // Set data
    void set(const qint32 &foreground_jersey_id, const qint32 &trim_jersey_layer_id);

private:
    qint32 foreground_jersey_id_{0};
    qint32 trim_jersey_id_{0};
};

#endif // DESIGN_ITEM_H
