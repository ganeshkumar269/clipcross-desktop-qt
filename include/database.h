#ifndef CUSTOM_DATABASE_H
#define CUSTOM_DATABASE_H
    
    #include <QSqlDatabase>
    #include <QSqlDriver>
    #include <QSqlError>
    #include <QSqlQuery>
    #include <QObject>
    #include "clip.h"
    //dbSchema : (id,value,format,timestamp)

    class Database:public QObject {
        Q_OBJECT

        public:

            explicit Database(const QString&,QObject* = nullptr);
            ~Database();

            // QList<QList<QVariant>>* retrieveClips(uy[[[[[[[[[[[[[[[[[[int=0,uint=100);
            QList<Clip>* retrieveClips(const QList<QString>&,uint=UINT32_MAX,uint=0);
            bool insertClip(const Clip&);//(val,format)
            bool insert(const QString,const QList<QString>&,const QList<QString>&);
            Clip getTopItem();

        private Q_SLOTS:
            void onOpened();

        private:
            const QString id;
            QSqlDatabase db;
    };
#endif
