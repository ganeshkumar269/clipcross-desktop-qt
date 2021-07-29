#include "utils.h"
#include <QUrl>
#include <QSysInfo>
#include <QSettings>
#include "log4qt/logger.h"
#include "constants.h"

QUrl getWssUrl(){
auto logger = Log4Qt::Logger::rootLogger(); 
    QString domain = API_WS_HOME_URL;
    logger->debug() << "[getWssUrl.cpp ] domain" << domain;
    QSettings s; 
    auto url = QUrl(domain + "/?id_token=" + s.value("id_token").toString() + "&device_id=" + s.value("device_id").toString());
    logger->debug() << __FILE__ << " url: " << url.toString();
    return url;
}