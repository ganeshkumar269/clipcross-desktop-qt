
#include <QApplication>
#include <QPushButton>
#include <QWidget>
#include <QLabel>
#include <QListView>
#include <QListWidget>
#include <QMainWindow>
#include <QSize>
#include <QObject>
#include <QClipboard>
#include <QHBoxLayout>
#include <QTableWidget>
#include <QTableView>
#include <string>
#include <QEvent>
#include <QMouseEvent>
#include <QCursor>
#include <QSizePolicy>
#include <vector>
#include <QStringListModel>
#include <QAbstractListModel>
#include <QCryptographicHash>
#include "database.h"
#include "utils.h"
// #include "clipboard.cpp"
int n = 250;


void handlethis(){
    qDebug() << "HandleTHis";
}



int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QWidget mainwindow;
    QVBoxLayout layout(&mainwindow);
    
    QPushButton addItemButton("Add button");
    QListView list;
    QStringListModel sm;
    
    sm.insertRow(0,sm.index(0));
    sm.setData(sm.index(0),QString("Test Text"));

    list.setModel(&sm);
    QFontMetrics qfm = mainwindow.fontMetrics();
    QByteArray prevHash = "";
    Database *db = new Database("QSQLITE");

    for(QString s : *(db->getItems())){
        sm.insertRow(0,sm.index(0));
        sm.setData(sm.index(0),s);
    } 
    // addItemButton.connect(&addItemButton,&QPushButton::pressed,&list,[&](){
    //     qDebug() << "Button Press" << '\n';
    //     addStringToModel(&sm,QString("Button Press String"),qfm,prevHash);
    // });

    QClipboard *clipboard = QGuiApplication::clipboard();

    QClipboard::connect(clipboard,
            QClipboard::dataChanged,
            [&](){handleDataChangedEvent(sm,clipboard,db,prevHash);}
        );


    layout.addWidget(&list);
    layout.addWidget(&addItemButton);

    mainwindow.resize(320, 240);    
    mainwindow.show();
    mainwindow.setWindowTitle("A Random Title");
    
    return a.exec();
}