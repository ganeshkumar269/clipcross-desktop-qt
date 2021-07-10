#include "utils.h"
#include <QSqlQuery>
#include <QSqlResult>

qint32 getRowCount(QString table){
    QSqlQuery q;
    QString query_string("select count(1) from ");
    query_string.append(table);
    q.prepare(query_string);
    if(!q.exec()){
        qDebug() << "getRowCount Failed to execute query: " << query_string;
        return -1;
    }
    else{
        qint32 val = q.value(0).toUInt();
        qDebug() << "getRowCount result " << val;
        return val;
    }
}