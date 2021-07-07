#ifndef CUSTOM_DATABASE_H
#define CUSTOM_DATABASE_H
    
    #include <QSqlDatabase>
    #include <QSqlDriver>
    #include <QSqlError>
    #include <QSqlQuery>

    //dbSchema : (textValue)

    void initDB();
    QStringList* getItems();
    bool insertItem(QString&);
    bool removeItem(int);

#endif