#include "jersey_image_server.h"

// Qt headers
#include <QComboBox>
#include <QDir>

// --- Static data --- //
JerseyImageServer JerseyImageServer::background_layers_;
JerseyImageServer JerseyImageServer::foreground_layers_;
JerseyImageServer JerseyImageServer::trim_layers_;
JerseyImageServer JerseyImageServer::preset_images_;
bool JerseyImageServer::static_jersey_servers_initialised_ = false;

// --- Constructor --- //
JerseyImageServer::JerseyImageServer() {}

// --- Populate and initialise combo box with jersey item names --- //
void JerseyImageServer::setComboBox(QComboBox *combo)
{
    const auto size{static_cast<qint32>(jerseys_.size())};
    for (qint32 i = 0; i < size; ++i) {
        // Row id numbers displayer with leading zeros e.g. 001, 002, etc
        combo->addItem(QString("%1 - %2").arg(i, 3, 10, QChar('0')).arg(jerseys_[i].displayName()));
    }
}

// --- Get file name of the chosen image --- //
QString JerseyImageServer::fileName(qint32 i) const
{
    if (!checkJersey(i)) {
        if (jerseys_.size() > 0)
            return jerseys_[0].fileName();

        return QString();
    }

    return jerseys_[i].fileName();
}

// --- Add a single image file --- //
qint32 JerseyImageServer::addFile(const QString &file_name, const QString &display_name)
{
    jerseys_.push_back(JerseyImageItem(file_name, display_name));

    return static_cast<qint32>(jerseys_.size());
}

// --- Add all image files from a folder --- //
qint32 JerseyImageServer::addFolder(const QString &folder_path)
{
    QDir dir(folder_path);
    if (!dir.exists())
        return -1;

    const auto list(dir.entryList(QStringList() << "*.png"
                                                << "*.PNG"
                                                << "*.svg"
                                                << "*.SVG",
                                  QDir::Files));

    for (const auto &itr : list)
        addFile(QString("%1/%2").arg(dir.absolutePath(), itr));

    return static_cast<qint32>(jerseys_.size());
}

// --- Sanity check the current jersey --- //
bool JerseyImageServer::checkJersey(qint32 i) const
{
    if (i >= 0 && i < static_cast<qint32>(jerseys_.size()))
        return true;

    return false;
}

// --- Get background layers jersey server --- //
JerseyImageServer &JerseyImageServer::backgroundLayers()
{
    if (!static_jersey_servers_initialised_)
        initStaticJerseyServers();

    return background_layers_;
}

// --- Get foreground layers jersey server --- //
JerseyImageServer &JerseyImageServer::foregroundLayers()
{
    if (!static_jersey_servers_initialised_)
        initStaticJerseyServers();

    return foreground_layers_;
}

// --- Get trim layers jersey server --- //
JerseyImageServer &JerseyImageServer::trimLayers()
{
    if (!static_jersey_servers_initialised_)
        initStaticJerseyServers();

    return trim_layers_;
}

// --- Get jersey image preset jersey server --- //
JerseyImageServer &JerseyImageServer::presetImages()
{
    if (!static_jersey_servers_initialised_)
        initStaticJerseyServers();

    return preset_images_;
}

// --- Initialise static jerseys --- //
void JerseyImageServer::initStaticJerseyServers()
{
    if (static_jersey_servers_initialised_)
        return;

    // Background layers
    background_layers_.addFile(":/images/default_background.png", QObject::tr("Default"));
    background_layers_.addFolder("layer_background");

    // Foreground layers
    foreground_layers_.addFile(":/images/default_foreground.png", QObject::tr("Default"));
    foreground_layers_.addFolder("layer_foreground");

    // Trim layers
    trim_layers_.addFile(":/images/default_trim.png", QObject::tr("Default"));
    trim_layers_.addFolder("layer_trim");

    // Presets
    preset_images_.addFile(":/images/default_preset_image.png", QObject::tr("Default"));
    preset_images_.addFolder("jersey_presets");

    static_jersey_servers_initialised_ = true;
}
