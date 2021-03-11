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
    static QString toSafeFilePath(const QString &file_path,
                                  QString file_name,
                                  const QString &file_extension);
    static void toSimpleString(QString &text);

    // Random seed
    inline qreal randomSeed() const { return random_seed_; }
    static inline void setRandomSeed(const qreal rs) { random_seed_ = rs; }
    static constexpr qreal MINIMUM_RANDOM_SEED{0.0};
    static constexpr qreal MAXIMUM_RANDOM_SEED{0.9999};

private:
    QString text_string_;

    // Random seed
    static qreal random_seed_;
};

#endif // TEXT_H
