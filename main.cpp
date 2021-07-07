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
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
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


    sm->insertRow(0,sm->index(0));
    sm->setData(sm->index(0),QString("Test Text"));
    
    // list->setFlow(QListView::Flow::TopToBottom);

    list->setModel(sm);
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
    mainwindow.resize(320, 240);    
    mainwindow.setWindowTitle("Clipboard Manager");
    mainwindow.show();
    
    return a.exec();
}