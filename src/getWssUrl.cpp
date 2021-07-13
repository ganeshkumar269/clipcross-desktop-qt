#include "utils.h"
#include <QUrl>
#include <QSysInfo>
#include <QSettings>

QUrl getWssUrl(){
    #ifdef LOCAL_WSS_TEST
        QString domain = "wss://localhost:3000/";
    #else
        QString domain = "wss://wss-node-server.ganeshkumar269.repl.co/"; 
    #endif
    qDebug() << "[getWssUrl.cpp ] domain" << domain;
    QSettings s; 
    auto url = QUrl(domain + "?id_token=" + s.value("id_token").toString() + "&device_id=" + s.value("device_id").toString());
    qDebug() << __FILE__ << " url: " << url.toString();
    return url;
}