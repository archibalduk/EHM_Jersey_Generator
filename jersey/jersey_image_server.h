#ifndef JERSEY_IMAGE_SERVER_H
#define JERSEY_IMAGE_SERVER_H

// Application headers
#include "jersey_image_item.h"

// Qt headers
class QComboBox;
#include <QHash>

// --- Jersey image file server --- //
class JerseyImageServer
{
public:
    JerseyImageServer();

    // Combo box
    void setComboBox(QComboBox *combo);

    // Get data
    qint32 find(const QString &name) const;
    qint32 find(const qreal &percentage) const;
    QString fileName(qint32 i) const;
    inline qint32 size() const { return static_cast<qint32>(jerseys_.size()); }

    // Static instances of the jersey server
    static JerseyImageServer &backgroundLayers();
    static JerseyImageServer &foregroundLayers();
    static JerseyImageServer &trimLayers();
    static JerseyImageServer &presetImages();

    enum ENUM_FLAGS {
        NO_RESULT = -1,
    };

private:
    // Available jerseys
    std::vector<JerseyImageItem> jerseys_;
    QHash<QString, qint32> jersey_name_list_;

    // Initialise/load data
    qint32 addFile(const QString &file_name, const QString &display_name = QString());
    qint32 addFolder(const QString &folder_path);
    void initHashTable();

    // Sanity check
    bool checkJersey(qint32 i) const;

    // Static instances of the jersey server
    static JerseyImageServer background_layers_;
    static JerseyImageServer foreground_layers_;
    static JerseyImageServer trim_layers_;
    static JerseyImageServer preset_images_;
    static bool static_jersey_servers_initialised_;

    static void initStaticJerseyServers();
};

#endif // JERSEY_IMAGE_SERVER_H
