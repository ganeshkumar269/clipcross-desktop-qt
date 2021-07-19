#ifndef UTILS_H
#define UTILS_H
    #include <QString>
    #include <QStringListModel>
    #include <QClipboard>
    #include <QByteArray>
    #include <QSqlDatabase>
    // #include "database.h"
//    void handleDataChangedEvent(QStringListModel&, QClipboard*, Database*,QByteArray&);
    QString formatQString(const QString&);
    QUrl getWssUrl();
    QUrl getOAuthUrl();
    QString getQueryParamValue(const QUrl&,const QString&);
    qint64 getTimestamp();
    qint32 getRowCount(QString);
    QString& quotedQString(QString&);
    QString getMd5Hash(const QString&);
    QSqlDatabase getDatabaseConnection(const QString&);
    // void Logger(QtMsgType, const QMessageLogContext&, const QString &);
#endif
