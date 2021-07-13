#include "utils.h"
#include <QUrl>
#include <QSettings>
QUrl getOAuthUrl(){
    QSettings s;
    QString rt, it;
    if(s.contains("refresh_token")){
        rt = s.value("refresh_token").toString();
        it = s.value("id_token").toString();
    }
    #ifdef LOCAL_WSS_TEST
        QString domain = "http://localhost:5000/api/signin?request_type=des&port=12121&refresh_token="+rt+"&id_token="+it;
    #else
        QString domain = "https://wss-node-server.ganeshkumar269.repl.co/auth"; 
    #endif
    qDebug() << "[getOAuthUrl.cpp ] domain " << domain;
    return QUrl(domain);
}