#include "utils.h"
#include <QUrl>
QUrl getOAuthUrl(){
    #ifdef LOCAL_WSS_TEST
        QString domain = "https://localhost:5000/api/signin?request_type=des&port=12121";
    #else
        QString domain = "https://wss-node-server.ganeshkumar269.repl.co/auth"; 
    #endif
    qDebug() << "[getOAuthUrl.cpp ] domain " << domain;
    return QUrl(domain);
}