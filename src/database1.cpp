#include "database.h"


#include <QSqlDatabase>
#include <QSqlDriver>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlResult>

void initDB(){
    const QString driver("QSQLITE");
    
    if(QSqlDatabase::isDriverAvailable(driver))
        qDebug() << "Drive Available";
    else 
        qDebug() << "Drive Available";

    QSqlDatabase db = QSqlDatabase::addDatabase(driver);
    db.setDatabaseName("sqliteDatabase");
    
    if(!db.open())
        qDebug() << "ERROR: " << db.lastError();
    else    
        qDebug() << "DB opened successfully";
        
    // for(QString table : db.tables())
    //     if(table == QString("newTable"))
    //         return;

    QSqlQuery query("CREATE TABLE IF NOT EXISTS newTable (name TEXT)");

    if(!query.isActive())
        qDebug() << "ERROR: " << query.lastError().text();
    else
        qDebug() << "Table Creation query success";
    
}

QStringList* getItems(){
    QSqlQuery query;
    query.prepare("SELECT name FROM newTable");
    // query.addBindValue(10);
    QStringList* sl = nullptr;
    if(!query.exec()){
        qDebug() << "ERROR: " << query.lastError().text();    
    }

    else{
        qDebug() << "select success";
        sl = new QStringList();
        while(query.next())
        {
            sl->append(query.value(0).toString());
        }
    }
    
    return sl;
}


bool insertItem(QString& item){
    QSqlQuery query;
    query.prepare("INSERT INTO newTable(name) VALUES(?)");
    query.addBindValue(item);
    // QString qStr = QString("INSERT INTO newTable(name) VALUES(?)");
    // qDebug() << "query: " << qStr;
    
    if(!query.exec()){
        qDebug() << "ERROR: " << query.lastError().text();
        return false;
    }
    else{
        qDebug() << "insert success";
        return true;
    }
}
