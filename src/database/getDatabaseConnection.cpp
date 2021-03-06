#include "utils.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include "log4qt/logger.h"


QSqlDatabase getDatabaseConnection(const QString& conn){
auto logger = Log4Qt::Logger::rootLogger(); 
    const QString driver("QSQLITE"); //dbName = QSQLITE
    if(QSqlDatabase::isDriverAvailable(driver))
        logger->debug() << "Driver Available";
    else 
        logger->debug() << "Driver Not Available";
    QSqlDatabase db = QSqlDatabase::addDatabase(driver,conn);
    db.setDatabaseName("sqliteDatabase");
    if(!db.open()){
        logger->debug() << "ERROR: " << db.lastError().text();
    }
    else{
        logger->debug() << "DB opened successfully"; //onopened slot
        QString query_string ="CREATE TABLE IF NOT EXISTS dbv3"
                            " (timestamp TEXT PRIMARY KEY DEFAULT(strftime(\'%s\','now') || substr(strftime(\'%f\','now'),4)),"
                            "value TEXT, format TEXT, hash TEXT)" ;
        QSqlQuery query(query_string);

        if(!query.isActive())
            logger->debug() << "query_string: "<<query_string <<  "\nERROR: " << query.lastError().text();
        else
            logger->debug() << "dbv2 Table Creation query success";
    }
}