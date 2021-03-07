#ifndef FONT_SERVER_H
#define FONT_SERVER_H

// Application headers
#include "../common/dimensions.h"
#include "font_item.h"

// Qt headers
class QComboBox;
#include <QFont>

// --- Jersey font server --- //
class FontServer : public QObject
{
    Q_OBJECT

public:
    FontServer();

    // Font
    QFont font(const qint32 point_size = Dimensions::JerseyNameFontSize,
               const qint32 weight = QFont::Thin) const;

    // Combo box
    void setComboBox(QComboBox *combo) const;

    // Get data
    qint32 currentFont() const;
    QString currentFontDisplayName() const;
    QString currentFontFamily() const;
    QString currentFontFileName() const;

public slots:
    // Set data
    void setFont(const qint32 id) const;

private:
    // Available fonts
    static std::vector<FontItem> fonts_;
    std::vector<FontItem> &fonts() const;

    // Current/selected font
    static qint32 current_font_id_;

    // Sanity check
    bool checkFont() const;
};

#endif // FONT_SERVER_H
