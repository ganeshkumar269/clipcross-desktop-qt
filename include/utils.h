#ifndef UTILS_H
#define UTILS_H

#include <QDateTime>
#include "log4qt/logger.h"
#include <QString>
#include <QStringListModel>
#include <QClipboard>
#include <QByteArray>
#include <QSqlDatabase>
#include <QUrl>
#include <QUrlQuery>
#include <QSqlQuery>
#include <QApplication>
#include <QFontMetricsF>
#include <QCryptographicHash>

QString formatQString(const QString& str);
QString getQueryParamValue(const QUrl& url,const QString& param);
qint32 getRowCount(const QString& table);
QString quotedQString(QString&str);
QString getMd5Hash(const QString& str);
QString sanitizeForSqlQuery(QString str);
qint64 getTimestamp();
#endif //CLIPPYCROSS_UTILS_H
