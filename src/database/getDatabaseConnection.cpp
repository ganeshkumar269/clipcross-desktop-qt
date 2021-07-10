#include "utils.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

QSqlDatabase getDatabaseConnection(const QString& conn){
    const QString driver("QSQLITE"); //dbName = QSQLITE
    if(QSqlDatabase::isDriverAvailable(driver))
        qDebug() << "Driver Available";
    else 
        qDebug() << "Driver Not Available";
    QSqlDatabase db = QSqlDatabase::addDatabase(driver,conn);
    db.setDatabaseName("sqliteDatabase");
    if(!db.open()){
        qDebug() << "ERROR: " << db.lastError();
    }
    else{
        qDebug() << "DB opened successfully"; //onopened slot
        QString query_string ="CREATE TABLE IF NOT EXISTS dbv3"
                            " (timestamp TEXT PRIMARY KEY DEFAULT(strftime(\'%s\','now') || substr(strftime(\'%f\','now'),4)),"
                            "value TEXT, format TEXT, hash TEXT)" ;
        QSqlQuery query(query_string);

        if(!query.isActive())
            qDebug() << "query_string: "<<query_string <<  "\nERROR: " << query.lastError().text();
        else
            qDebug() << "dbv2 Table Creation query success";
    }
}