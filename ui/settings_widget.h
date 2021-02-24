#ifndef SETTINGS_WIDGET_H
#define SETTINGS_WIDGET_H

// Application headers
#include "../dimensions.h"
#include "../jersey/font_server.h"

// Qt headers
class QComboBox;
class QGroupBox;
class QSpinBox;
#include <QWidget>

// --- Settings widget --- //
class SettingsWidget : public QWidget
{
public:
    SettingsWidget(QWidget *parent = nullptr);

private:
    // Font
    QGroupBox *createFontGroup();
    QComboBox *font_selector_;
    FontServer font_server_;

    // Text position
    QGroupBox *createTextPositionGroup();
    QSpinBox *name_text_vertical_position_offset_;
    QSpinBox *number_text_vertical_position_offset_;

    enum ENUM_RANGES {
        VERTICAL_POSITION_ADJUST
        = 50, // Reduce the full range as lesser values will still allow text to go off-screen
        MINIMUM_VERTICAL_POSITION_OFFSET = 0
                                           - (Dimensions::JerseyImageHeight
                                              - VERTICAL_POSITION_ADJUST),
        MAXIMUM_VERTICAL_POSITION_OFFSET = Dimensions::JerseyImageHeight - VERTICAL_POSITION_ADJUST
    };

private slots:
    // Text position
    void setNameTextVerticalPositionOffset(const qint32 i);
    void setNumberTextVerticalPositionOffset(const qint32 i);
};

#endif // SETTINGS_WIDGET_H
