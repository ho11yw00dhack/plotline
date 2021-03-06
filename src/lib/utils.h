#ifndef UTILS_H
#define UTILS_H

#include <QBuffer>
#include <QImage>
#include <QJsonValue>
#include <QPixmap>
#include <QtDebug>
#include <QtCore>
#include <QtWidgets/QLayout>
#include <QtWidgets/QWidget>

// Thanks to http://stackoverflow.com/a/32378165

QJsonValue jsonValFromPixmap(const QPixmap &p);
QJsonValue jsonValFromImage(const QImage &i);
QPixmap pixmapFrom(const QJsonValue & val);
QImage imageFrom(const QJsonValue &val);
QString friendlyList(const QStringList &stringList);

// Thanks to http://stackoverflow.com/a/7077340
void clearLayout(QLayout* layout, bool deleteWidgets = true);
void setEnabledRecursive(QLayout *layout, bool enabled);
void setDisabledRecursive(QLayout *layout, bool enabled);

int findCharReverse(const QString &chars, const QString &s, const int from,
                    const QString &escape=QString());
QString substring(const QString &string, int start, int end=-1);
QPair<int, int> multilineOffset(QString string, int offset);
QString reflowParagraph(const QString &paragraph, const int width);
QString reflowParagraphs(const QString &paragraph, const int width);

// Number conversion (intended for chapters)
QString toRomanNumeral(const int number);
QString toWord(const int number);

#endif // UTILS_H
