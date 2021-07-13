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
#include <QLabel>
#include <QFontDatabase>
#include <QPalette>
#include <QBrush>
#include "framelesswindow\framelesswindow.h"

#include <iostream>

int main(int argc, char *argv[])
{
    qInstallMessageHandler(Logger);
    QApplication a(argc, argv);
    FramelessWindow framelesswindow;
    QWidget mainwindow;
    QVBoxLayout *layout = new QVBoxLayout(&mainwindow);
    QHBoxLayout *dirButtons = new QHBoxLayout();
    QHBoxLayout *loginLogoutButtons = new QHBoxLayout();
    qDebug() << "[main.cpp] main qDebug Works"; 
    std::cerr << "This is from standard error" << std::endl; 
    std::cout << "This is from standard output" << std::endl; 
    QListView *list = new QListView();
    QLabel* vcbId = new QLabel();
    // QPushButton *button = new QPushButton("Sign In from Google");
    QPushButton *right = new QPushButton(">>");
    QPushButton *left = new QPushButton("<<");
    QPushButton *login = new QPushButton("Login");
    QPushButton *logout = new QPushButton("Logout");
    QStringListModel *sm = new QStringListModel();

    QGuiApplication::setOrganizationName("TestOrgName");
    QGuiApplication::setOrganizationDomain("TestOrgDomainName.com");
    QGuiApplication::setApplicationName("Clipboard Manager");

    QPalette defaultPalette;
    QBrush base; base.setColor(QColor("#283742"));
    QBrush alternateBase; alternateBase.setColor(QColor("#6784a3"));
    defaultPalette.setBrush(QPalette::Base,base);
    defaultPalette.setBrush(QPalette::AlternateBase,alternateBase);

    QFontDatabase::addApplicationFont("./resources/Roboto-Medium.ttf");

    sm->insertRow(0,sm->index(0));
    sm->setData(sm->index(0),QString("Test Text"));

    list->setModel(sm);
    list->setPalette(defaultPalette);
    list->setAlternatingRowColors(true);

    vcbId->setText("Test VCB");
    
    Handler handler;  

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
    
    handler.connect(&handler,&Handler::updateVcbId,vcbId,[&](QString vcbIdString){
        vcbId->setText(vcbIdString);
    });

    left->connect(left,&QPushButton::clicked,&handler,[&](){
        handler.goPrevious();
    });

    right->connect(right,&QPushButton::clicked,&handler,[&](){
        handler.goNext();
    });

    login->connect(login, &QPushButton::clicked, &handler,&Handler::startLogin);
    logout->connect(logout, &QPushButton::clicked, &handler,&Handler::startLogout);

    dirButtons->addWidget(left);
    dirButtons->addWidget(right);
    loginLogoutButtons->addWidget(login);
    loginLogoutButtons->addWidget(logout);
    layout->addLayout(dirButtons);
    layout->addLayout(loginLogoutButtons);
    layout->addWidget(list);
    layout->addWidget(vcbId);

    mainwindow.setStyleSheet("background-color: #283742; color: #aaccff; font-family : roboto");
    mainwindow.resize(360, 440);    
    mainwindow.setPalette(defaultPalette);

    framelesswindow.setWindowTitle("clipcross");
    framelesswindow.setContent(&mainwindow);
    framelesswindow.setPalette(defaultPalette);
    framelesswindow.show();
    
    return a.exec();
}