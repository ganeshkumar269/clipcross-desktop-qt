#include "utils.h"
#include <QUrl>
#include <QSettings>
#include "constants.h"
QUrl getOAuthUrl(){
    QSettings s;
    QString rt, it;
    if(s.contains("refresh_token")){
        rt = s.value("refresh_token").toString();
        it = s.value("id_token").toString();
    }
    QString domain = WEB_SIGNIN_URL + "?request_type=des&port=12121&refresh_token="+rt+"&id_token="+it;
    qDebug() << "[getOAuthUrl.cpp ] domain " << domain;
    return QUrl(domain);
}