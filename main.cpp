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


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    FramelessWindow framelesswindow;
    QWidget mainwindow;
    QVBoxLayout *layout = new QVBoxLayout(&mainwindow);
    QHBoxLayout *buttons = new QHBoxLayout();
    qDebug() << "[main.cpp] main qDebug Works"; 
    QListView *list = new QListView();
    QLabel* vcbId = new QLabel();
    // QPushButton *button = new QPushButton("Sign In from Google");
    QPushButton *right = new QPushButton(">");
    QPushButton *left = new QPushButton("<");
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
    QFont* mainFont = new QFont("Roboto");
    mainFont->setBold(true);
    // mainFont->setPointSize(12);


    sm->insertRow(0,sm->index(0));
    sm->setData(sm->index(0),QString("Test Text"));
    
    // list->setFlow(QListView::Flow::TopToBottom);

    list->setModel(sm);
    list->setFont(*mainFont);
    list->setPalette(defaultPalette);
    // list->setWrapping(true);
    list->setAlternatingRowColors(true);

    vcbId->setText("Test VCB");
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
    
    handler.connect(&handler,&Handler::updateVcbId,vcbId,[&](QString vcbIdString){
        vcbId->setText(vcbIdString);
    });
    left->connect(left,&QPushButton::clicked,&handler,[&](){
        handler.goPrevious();
    });
    right->connect(right,&QPushButton::clicked,&handler,[&](){
        handler.goNext();
    });

    buttons->addWidget(left);
    buttons->addWidget(right);
    layout->addLayout(buttons);
    layout->addWidget(list);
    layout->addWidget(vcbId);


    mainwindow.setFont(*mainFont);
    mainwindow.setPalette(defaultPalette);

    mainwindow.setStyleSheet("background-color: #283742; color: #b73225");
    mainwindow.resize(360, 440);    
    // mainwindow.setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    mainwindow.setWindowTitle("Clipboard Manager");
    framelesswindow.setContent(&mainwindow);
    framelesswindow.show();
    // mainwindow.show();
    
    return a.exec();
}