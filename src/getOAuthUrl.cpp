#include "utils.h"
#include <QUrl>
QUrl getOAuthUrl(){
    #ifdef LOCAL_WSS_TEST
        QString domain = "https://localhost:3000/auth";
    #else
        QString domain = "https://wss-node-server.ganeshkumar269.repl.co/auth"; 
    #endif
    qDebug() << "[getOAuthUrl.cpp ] domain" << domain;
    // QSettings s;
    return QUrl(domain);
}