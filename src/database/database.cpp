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
#include "log4qt/logger.h"
QT_USE_NAMESPACE

// auto logger = Log4Qt::Logger::rootLogger(); 

Database::Database(const QString& _id,QObject* parent):id(_id),QObject(parent){
    const QString driver("QSQLITE"); //dbName = QSQLITE
    if(QSqlDatabase::isDriverAvailable(driver))
        qDebug() << "Driver Available";
    else 
        qDebug() << "Driver Not Available";
    db = QSqlDatabase::addDatabase(driver,id);
    db.setDatabaseName("testdatabase");
    if(!db.open())
        qDebug() << "ERROR: " << db.lastError().text();
    else{
        qDebug() << "DB opened successfully"; //onopened slot
        QString query_string ="CREATE TABLE IF NOT EXISTS " + id + 
                            " (timestamp TEXT PRIMARY KEY DEFAULT(strftime(\'%s\','now') || substr(strftime(\'%f\','now'),4)),"
                            "value TEXT, format TEXT, hash TEXT)" ;
        
        // query.prepare(query_string);
        auto query = db.exec(query_string);

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

QList<Clip>* Database::retrieveClips(const QList<QString>& cols,uint limit,uint offset){
    QList<Clip>* res = new QList<Clip>();
    QString limit_string = "";
    // QString qms("");
    // if(!(cols.size() == 1 && cols.at(0).trimmed() == "*")){
    //     for(int i = 0; i < cols.size(); i++) qms.append(cols.at(i)+ (i == cols.size() - 1 ? " " : ", "));
    // }
    // else {
    //     qms = "*";
    // }
    if(limit != UINT32_MAX) limit_string = " LIMIT " + QString::number(limit) + "," + QString::number(offset);
    QString query_string = "SELECT * FROM " + id + " ORDER BY timestamp DESC " + limit_string;
    
    
    QSqlQuery query = db.exec(query_string);
    if(!query.isActive()){
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


bool Database::insertClip(const Clip& clip){
    auto query = db.exec("insert into " + id + " values(" + 
                        QString::number(clip.timestamp()) + 
                        ",\"" + clip.value() + 
                        "\",\"" + clip.format() +
                        "\",\"" + clip.hash() +"\")"
                        );    

    // db.exec();
    if(!query.isActive()){
        qDebug() << "database.cpp Database::insertClip query: "<< query.lastQuery();
        qDebug() << "ERROR: " << query.lastError().text();
        // qDebug() << "Error lastquery:" << query.lastQuery();
        return false;
    }
    else{
        qDebug() << "database.cpp Database::insertClip query: "<< query.lastQuery();
        qDebug() << "database.cpp Database::insertClip insert success";
        return true;
    }
}

bool Database::insert(const QString table,const QList<QString>& cols,const QList<QString>& values){
    QSqlQuery query(db);

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

Clip Database::getTopItem(){
    return retrieveClips({},1)->at(0);
}

void Database::onOpened(){}
