#include <QApplication>
#include <QWidget>
#include <QListView>
#include <QStringListModel>
#include <QVBoxLayout>
#include <QPushButton>
#include "database.h"
#include "utils.h"
#include "handler.h"
#include "authenticate.h"
#include <QSettings>
#include <QObject>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QWidget mainwindow;
    QVBoxLayout *layout = new QVBoxLayout(&mainwindow);
    qDebug() << "[main.cpp] main qDebug Works"; 
    QListView *list = new QListView();
    // QPushButton *button = new QPushButton("Sign In from Google");
    QPushButton *right = new QPushButton(">");
    QPushButton *left = new QPushButton("<");
    QStringListModel *sm = new QStringListModel();

    QGuiApplication::setOrganizationName("TestOrgName");
    QGuiApplication::setOrganizationDomain("TestOrgDomainName.com");
    QGuiApplication::setApplicationName("Clipboard Manager");


    sm->insertRow(0,sm->index(0));
    sm->setData(sm->index(0),QString("Test Text"));

    list->setModel(sm);

    list->setWrapping(true);
    list->setAlternatingRowColors(true);
    // Authenticate authenticate;
    // button->connect(button,&QPushButton::clicked,button,[&](){
    //     qDebug() << "Clicked";
    //     authenticate.start();
    // });
    // list->setVisible(false);
    
    Handler handler;  
    // handler.setSM(sm);   
    list->connect(list,&QAbstractItemView::doubleClicked,&handler,
        [&](const QModelIndex& ind){
            qDebug()<<"Double Clicked";
            qDebug() << sm->data(ind);
    });
    handler.connect(&handler,&Handler::updateListViewModel,list,[&](QStringListModel* slm){
        qDebug() << "ListViewModel updated";
        list->setModel(slm);
        qDebug() << "Slm" << slm->data(slm->index(0));
    });
    
    left->connect(left,&QPushButton::clicked,&handler,[&](){
        handler.goPrevious();
    });
    right->connect(right,&QPushButton::clicked,&handler,[&](){
        handler.goNext();
    });
    // handler.goNext();
    // authenticate.connect(&authenticate,&Authenticate::authenticated,&authenticate,[&](){
    //     qDebug() << "Authentication Success" ;
    //     QSettings s;
    //     qDebug() << s.value("access_token");
    //     qDebug() << s.value("refresh_token");
    //     button->setVisible(false);
    //     list->setVisible(true);
    //     handler = new Handler(nullptr);
    //     handler->setSM(sm);
    //     handler->connect(handler,&Handler::tokenExpired,&authenticate,[&](){
    //         authenticate.refreshAccessToken();
    //     });
    //     authenticate.connect(&authenticate,&Authenticate::accessTokenRefreshed,
    //         handler,[&](){
    //             handler->initWsw();
    //         });
    // });

    // Q_UNUSED(handler);
    
    // layout->addWidget(button);
    layout->addWidget(left);
    layout->addWidget(right);
    layout->addWidget(list);
    mainwindow.resize(320, 240);    
    mainwindow.setWindowTitle("Clipboard Manager");
    mainwindow.show();
    
    return a.exec();
}