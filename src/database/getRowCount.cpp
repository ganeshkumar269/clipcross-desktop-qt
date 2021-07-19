#include "utils.h"
#include <QSqlQuery>
#include <QSqlResult>
#include "log4qt/logger.h"

qint32 getRowCount(QString table){
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