#include "utils.h"
#include <QUrl>
#include <QUrlQuery>
#include "log4qt/logger.h"
// auto logger = Log4Qt::Logger::rootLogger(); 

QString getQueryParamValue(const QUrl& url,const QString& param){
    QUrlQuery q(QUrl(QUrl::fromPercentEncoding(url.toString().toLocal8Bit())).query());
    if(q.hasQueryItem(param))
        return q.queryItemValue(param);
    else {
        qDebug() <<  " Param doesnt exist";
        return "";
    }
}