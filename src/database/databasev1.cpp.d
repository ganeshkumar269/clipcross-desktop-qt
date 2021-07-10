#include "database.h"


#include <QSqlDatabase>
#include <QSqlDriver>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlResult>
#include <QDateTime>
#include <QSqlRecord>
#include <QSqlField>
#include "utils.h"

QT_USE_NAMESPACE

Database::Database(QObject* parent):QObject(parent){
    const QString driver("QSQLITE"); //dbName = QSQLITE
    if(QSqlDatabase::isDriverAvailable(driver))
        qDebug() << "Driver Available";
    else 
        qDebug() << "Driver Not Available";
    db = QSqlDatabase::addDatabase(driver);
    db.setDatabaseName("sqliteDatabase");
    if(!db.open())
        qDebug() << "ERROR: " << db.lastError();
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

Database::~Database(){
    qDebug() << "DB Connection Closed";
    db.close();
    //onclose slot
}

QList<Clip>* Database::retrieveClips(const QString table,const QList<QString>& cols,uint limit,uint offset){
    QList<Clip>* res = new QList<Clip>();
    QSqlQuery query;
    QString limit_string = "";
    // QString qms("");
    // if(!(cols.size() == 1 && cols.at(0).trimmed() == "*")){
    //     for(int i = 0; i < cols.size(); i++) qms.append(cols.at(i)+ (i == cols.size() - 1 ? " " : ", "));
    // }
    // else {
    //     qms = "*";
    // }
    if(limit != UINT32_MAX) limit_string = " LIMIT " + QString::number(limit) + "," + QString::number(offset);
    QString query_string = "SELECT * FROM " + table + " ORDER BY timestamp DESC " + limit_string;
    
    
    query.prepare(query_string);
    if(!query.exec()){
        qDebug() << "database.cpp Database::retrieve query: " << query_string;
        qDebug() << "database.cpp Database::retrieve failed ERROR: " << query.lastError().text();    
    }
    else{
        qDebug() << "database.cpp Database::retrieve query: " << query_string;
        qDebug() << "database.cpp Database::retrieve success";
        uint cols_cnt = query.record().count();
        while(query.next())
        { 
            // QList<QVariant> res_row(cols_cnt); //optimisation possible here
            Clip temp(query.value(1).toString(),query.value(2).toString(),query.value(0).toLongLong());
            res->append(temp);
        }
    }

    return res;
}

// QList<QList<QVariant>>* Database::retrieveClips(uint index,uint cnt){
//     QList<QList<QVariant>>* res = nullptr;
//     return retrieve("dbv2",{"value","format"});
//     // return res;
// }



bool Database::insertClip(const Clip& clip){
    // return insert("dbv2",{"timestamp","value","format","hash"},
    //         {clip.timestamp,clip.value,clip.format,clip.hash});

    // QString cols_string(""),values_string("");
    // for(int i = 0; i < cols.size(); i++) 
    //     cols_string.append("\""+ cols.at(i) + "\"" + (i == cols.size() - 1 ? " " : ","));
    // for(int i = 0; i < values.size(); i++) 
    //     values_string.append("\"" + values.at(i) + "\"" + (i == values.size() - 1 ? " " : ","));
    
    QSqlQuery query;

    QString query_string ="INSERT INTO dbv3 VALUES(?,?,?,?)"; 
    query.prepare(query_string);
    query.addBindValue(clip.timestamp());
    query.addBindValue(clip.value());
    query.addBindValue(clip.format());
    query.addBindValue(clip.hash());
    if(!query.exec()){
        qDebug() << "database.cpp Database::insertClip query: "<< query_string;
        qDebug() << "ERROR: " << query.lastError().text();
        return false;
    }
    else{
        qDebug() << "database.cpp Database::insertClip query: "<< query_string;
        qDebug() << "database.cpp Database::insertClip insert success";
        return true;
    }
}

bool Database::insert(const QString table,const QList<QString>& cols,const QList<QString>& values){
    QSqlQuery query;

    QString cols_string(""),values_string("");
    for(int i = 0; i < cols.size(); i++) 
        cols_string.append("\""+ cols.at(i) + "\"" + (i == cols.size() - 1 ? " " : ","));
    for(int i = 0; i < values.size(); i++) 
        values_string.append("\"" + values.at(i) + "\"" + (i == values.size() - 1 ? " " : ","));
    

    QString query_string ="INSERT INTO " + table + "(" + cols_string + ")" + " VALUES(" + values_string + ")"; 

    query.prepare(query_string);
    
    if(!query.exec()){
        qDebug() << "database.cpp Database::insert query: "<< query_string;
        qDebug() << "ERROR: " << query.lastError().text();
        return false;
    }
    else{
        qDebug() << "database.cpp Database::insert query: "<< query_string;
        qDebug() << "database.cpp Database::insert insert success";
        return true;
    }
}

void Database::onOpened(){}
