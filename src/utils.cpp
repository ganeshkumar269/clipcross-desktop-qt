#include "utils.h"
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
// #include <windows.h>
// #include "database.h"
//    void handleDataChangedEvent(QStringListModel&, QClipboard*, Database*,QByteArray&);
QString formatQString(const QString& str){
    std::string no_newlines = str.toStdString();
    for(char &i : no_newlines)if(i == '\n')i = ' ';
    return QFontMetricsF(QApplication::font()).elidedText(no_newlines.c_str(),Qt::ElideRight,430);
}

QString getQueryParamValue(const QUrl& url,const QString& param){
    QUrlQuery q(QUrl(QUrl::fromPercentEncoding(url.toString().toLocal8Bit())).query());
    if(q.hasQueryItem(param))
        return q.queryItemValue(param);
    else {
        qDebug() <<  " Param doesnt exist";
        return "";
    }
}

qint64 getTimestamp(){return QDateTime::currentMSecsSinceEpoch();}

qint32 getRowCount(const QString& table){
    auto logger = Log4Qt::Logger::rootLogger();
    QSqlQuery q;
    QString query_string("select count(1) from ");
    query_string.append(table);
    q.prepare(query_string);
    if(!q.exec()){
        logger->debug() << "getRowCount Failed to execute query: " << query_string;
        return -1;
    }
    else{
        qint32 val = q.value(0).toUInt();
        logger->debug() << "getRowCount result " << val;
        return val;
    }
}

QString quotedQString(QString&str){
    str.prepend("\"");
    str.append("\"");
    return str;
}

QString getMd5Hash(const QString& str){
    QCryptographicHash qh(QCryptographicHash::Md5);
    qh.addData(str.toStdString().c_str(),16);
    return qh.result().toHex();
}

//QSqlDatabase getDatabaseConnection(const QString&);
// void Logger(QtMsgType, const QMessageLogContext&, const QString &);
// void simulatePasteKeySequence(){
//     INPUT ip;
//     ip.type = INPUT_KEYBOARD;
//     ip.ki.wScan = 0;
//     ip.ki.time = 0;
//     ip.ki.dwExtraInfo = 0;
//     // Press the "Ctrl" key
//     ip.ki.wVk = VK_CONTROL;
//     ip.ki.dwFlags = 0; // 0 for key press
//     SendInput(1, &ip, sizeof(INPUT));
//     // Press the "V" key
//     ip.ki.wVk = 'V';
//     ip.ki.dwFlags = 0; // 0 for key press
//     SendInput(1, &ip, sizeof(INPUT));
//     // Release the "V" key
//     ip.ki.wVk = 'V';
//     ip.ki.dwFlags = KEYEVENTF_KEYUP;
//     SendInput(1, &ip, sizeof(INPUT));
//     // Release the "Ctrl" key
//     ip.ki.wVk = VK_CONTROL;
//     ip.ki.dwFlags = KEYEVENTF_KEYUP;
//     SendInput(1, &ip, sizeof(INPUT));
// }

QString sanitizeForSqlQuery(QString str){
    str.replace("/", "\\/");
    return str;
}
