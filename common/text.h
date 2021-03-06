#ifndef TEXT_H
#define TEXT_H

// Qt headers
#include <QString>

// --- Text manipulation --- //
class Text
{
public:
    Text(const QString &text);

    // Numerical functions
    qreal averageCharacterValuePercentage() const;

    // Text functions
    QString simpleString() const;
    QString simpleStringLowerCase() const { return simpleString().toLower(); }
    inline QString text() const { return text_string_; }
    static void toSimpleString(QString &text);

private:
    QString text_string_;
};

#endif // TEXT_H
