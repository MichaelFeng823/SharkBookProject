#ifndef CHINESE2PINYIN_H
#define CHINESE2PINYIN_H

#include <QJsonDocument>
#include <QVariant>
#include <QMap>
#include <QObject>


class Chinese2Pinyin:public QObject
{
     Q_OBJECT
public:
    explicit Chinese2Pinyin(bool polyPhone = false, QObject *parent = 0);

public:
    static QString getFullChars(QString input);
    static QStringList getCamelChars(QString str);
    static void setPolyPhone(bool polyPhone);
    static bool polyPhone();

private:
    static QMap <QString, QVariant> m_fullDict;
    static QMap <QString, QVariant> m_polyphone;
    static bool m_checkPolyphone;
    static QString m_charDict;

private:
    static QString getFullChar(QString str);
    static QString capitalize(QString str);
    static QString getChar(QChar ch);
    static QStringList getResult(QStringList chars);
};

#endif // CHINESE2PINYIN_H
