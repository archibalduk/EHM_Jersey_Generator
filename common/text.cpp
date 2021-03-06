#include "text.h"

// Qt headers
#include <QRegularExpression>

// --- Constructor --- //
Text::Text(const QString &text) : text_string_(text) {}

// --- Calculate the average char value of the text as a pecentage relative to the max ASCII value--- //
qreal Text::averageCharacterValuePercentage() const
{
    const auto size{static_cast<qint32>(text_string_.size())};
    qint32 sum{0};

    for (qint32 i = 0; i < size; ++i)
        sum += static_cast<qint32>(text_string_[i].toLatin1());

    const auto average{static_cast<qreal>(sum) / static_cast<qreal>(size)};

    return average - floor(average);
}

// --- Return the text as a simple string --- //
QString Text::simpleString() const
{
    QString text(text_string_);
    toSimpleString(text);
    return text;
}

// --- Convert text to a simple string with no accents --- //
void Text::toSimpleString(QString &text)
{
    text = text.trimmed();

    // Remove characters to be ignored
    text.remove(QChar(0x2122)); // Trademark

    // Replace characters not corrected by normalising
    // and replace any underscores with spaces to allow file names with underscores to be matched
    // against team names, etc.
    text.replace("ø", "o");
    text.replace("_", " ");

    // Remove accents and convert for Latin-1
    text = text.normalized(QString::NormalizationForm_KD)
               .remove(QRegularExpression("^a-zA-Z\\s\\d_:"))
               .toLatin1();

    // Remove any question marks left over from conversion to Latin-1
    text = text.remove("?");
}
