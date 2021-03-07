#include "font_server.h"

// Qt headers
#include <QComboBox>
#include <QDir>
#include <QSettings>

// --- Static data --- //
qint32 FontServer::current_font_id_{0};
std::vector<FontItem> FontServer::fonts_;

/* ===================== */
/*      Font Server      */
/* ===================== */

// --- Constructor --- //
FontServer::FontServer()
{
    QSettings settings;
    current_font_id_ = settings.value("selected_font", 0).toInt();
    checkFont();
}

/* ================== */
/*      Get Data      */
/* ================== */

// --- Get current font display name --- //
QString FontServer::currentFontDisplayName() const
{
    checkFont();
    return fonts()[current_font_id_].displayName();
}

// --- Get current font family --- //
QString FontServer::currentFontFamily() const
{
    checkFont();
    return fonts()[current_font_id_].fontFamily();
}

// --- Get current font file name --- //
QString FontServer::currentFontFileName() const
{
    checkFont();
    return fonts()[current_font_id_].fileName();
}

// --- Get current font id --- //
qint32 FontServer::currentFont() const
{
    checkFont();
    return current_font_id_;
}

// --- Create and return the font --- //
QFont FontServer::font(const qint32 point_size, const qint32 weight) const
{
    QFont font(currentFontFamily(), point_size, weight);
    font.setLetterSpacing(QFont::AbsoluteSpacing, Dimensions::FontLetterSpacing);
    font.setStyleStrategy(QFont::PreferAntialias);
    font.setStretch(QFont::Unstretched);

    return font;
}

// --- Get list of available fonts --- //
std::vector<FontItem> &FontServer::fonts() const
{
    // Populate the list of fonts if it is empty
    if (fonts_.size() < 1) {
        // Face Off M54 is a freeware font by justme54s - https://www.dafont.com/face-off-m54.font
        fonts_.push_back(FontItem(":/fonts/face_off_m54.ttf", false));
        // Jersey M54 is a freeware font by justme54s - https://www.dafont.com/jersey-m54.font
        fonts_.push_back(FontItem(":/fonts/jersey_m54.ttf", false));
        // New Athletic M54 is a freeware font by justme54s - https://www.dafont.com/new-athletic-m54.font
        fonts_.push_back(FontItem(":/fonts/new_athletic_m54.ttf", false));
        // Sports Jersey is a freeware font by Arfurware - https://www.dafont.com/sports-jersey.font
        fonts_.push_back(FontItem(":/fonts/sports_jersey.ttf", false));
        // Sports World is a freeware font by Sergiy S. Tkachenko - https://www.dafont.com/sports-world.font
        fonts_.push_back(FontItem(":/fonts/sports_world.ttf", false));

        // Find any fonts in the 'fonts' sub-folder
        QDir dir("extra_fonts");
        if (dir.exists()) {
            const auto extra_fonts_list(dir.entryList(QStringList() << "*.ttf"
                                                                    << "*.TTF",
                                                      QDir::Files));

            for (const auto &itr : extra_fonts_list) // Assume that all fonts are accent-compatible
                fonts_.push_back(FontItem(QString("%1/%2").arg(dir.absolutePath(), itr), true));
        }
    }

    return fonts_;
}

/* ================== */
/*      Set Data      */
/* ================== */

// --- Populate and initialise combo box with font names --- //
void FontServer::setComboBox(QComboBox *combo) const
{
    const auto list{fonts()};
    for (const auto &itr : list) {
        combo->addItem(itr.displayName());
    }

    combo->setCurrentIndex(currentFont());
    QObject::connect(combo, &QComboBox::currentIndexChanged, this, &FontServer::setFont);
}

// --- Set font --- //
void FontServer::setFont(const qint32 id) const
{
    current_font_id_ = id;
    checkFont();

    QSettings settings;
    settings.setValue("selected_font", current_font_id_);
}

/* ==================== */
/*      Validation      */
/* ==================== */

// --- Sanity check the current font --- //
bool FontServer::checkFont() const
{
    if (current_font_id_ >= 0 && current_font_id_ < static_cast<qint32>(fonts().size()))
        return true;

    current_font_id_ = 0;
    return false;
}
