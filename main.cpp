#include <QApplication>
#include <QWidget>
#include <QListView>
#include <QStringListModel>
#include <QVBoxLayout>
#include <QPushButton>
#include "database.h"
#include "utils.h"
#include "authenticate.h"
#include <QSettings>
#include <QObject>
#include <QLabel>
#include <QIcon>
#include <QFontDatabase>
#include <QPalette>
#include <QBrush>
#include "framelesswindow\framelesswindow.h"
#include "framelesswindow/DarkStyle.h"
#include <QLoggingCategory>
#include <iostream>
#include <QSystemTrayIcon>
#include <QStyle>
#include <QAction>
#include <QMenu>
#include <QAbstractItemView>
#include "log4qt/logger.h"
#include "log4qt/propertyconfigurator.h"
#include "log4qt/loggerrepository.h"
#include "log4qt/consoleappender.h"
#include "log4qt/ttcclayout.h"
#include "log4qt/logmanager.h"
#include "log4qt/fileappender.h"
#define WINVER 0x0500
#include <windows.h>
#include "mainwindow.h"
#include "handler.h"
Q_LOGGING_CATEGORY(category1, "test.category1")

void setUpLogger(){
    auto logger = Log4Qt::Logger::rootLogger();
    auto *layout = new Log4Qt::TTCCLayout();
    layout->setName(QStringLiteral("My Layout"));
    layout->activateOptions();
    // Create a console appender
    Log4Qt::ConsoleAppender *consoleAppender = new Log4Qt::ConsoleAppender(layout, Log4Qt::ConsoleAppender::STDOUT_TARGET);
    consoleAppender->setName(QStringLiteral("My Appender"));
    consoleAppender->activateOptions();
    // Add appender on root logger
    logger->addAppender(consoleAppender);
    // Create a file appender
    Log4Qt::FileAppender *fileAppender = new Log4Qt::FileAppender(layout, QCoreApplication::applicationDirPath() + "/main.log", true);
    fileAppender->setName(QStringLiteral("My file appender"));
    fileAppender->activateOptions();
    // Add appender on root logger
    logger->addAppender(fileAppender);
    // Set level to info
    logger->setLevel(Log4Qt::Level::DEBUG_INT);
}

void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QByteArray localMsg = msg.toLocal8Bit();
    const char *file = context.file ? context.file : "";
    const char *function = context.function ? context.function : "";
    auto logger = Log4Qt::Logger::rootLogger();
    switch (type) {
    case QtDebugMsg:
        logger->debug() << file << " " << function << ":" << context.line  << " " << localMsg.constData();
        break;
    case QtInfoMsg:
        logger->info() << file << " " << function << ":" << context.line  << " " << localMsg.constData();
        break;
    case QtWarningMsg:
        logger->warn() << file << " " << function << ":" << context.line  << " " << localMsg.constData();
        break;
    case QtCriticalMsg:
        logger->error() << file << " " << function << ":" << context.line  << " " << localMsg.constData();
        break;
    case QtFatalMsg:
        logger->fatal() << file << " " << function << ":" << context.line  << " " << localMsg.constData();
        break;
    }
}

class KeyPressEater : public QObject
{
    // Q_OBJECT
    protected:
        bool eventFilter(QObject *obj, QEvent *event) override;
};

bool KeyPressEater::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::MouseButtonPress) {
        // QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        // qDebug("Ate key press %d", keyEvent->key());
        // return true;
        qDebug() << "There is a mouse press";
        return true;
    } else {
        // standard event processing
        return QObject::eventFilter(obj, event);
    }
    return true;
}



int main(int argc, char *argv[])
{
    qInstallMessageHandler(myMessageOutput);
    QApplication a(argc, argv);
    setUpLogger();
    const QPoint windowSize(360,440);
    MainWindow *mainwindow = new MainWindow();

    qDebug() << "[main.cpp] main logger->debug Works"; 
    std::cerr << "This is from standard error" << std::endl; 
    std::cout << "This is from standard output" << std::endl; 
    auto logger = Log4Qt::Logger::rootLogger();
    logger->debug() << "This is debug";

    QApplication::setStyle(new DarkStyle);
    FramelessWindow framelesswindow;

    QVBoxLayout *layout = new QVBoxLayout(mainwindow);
    QHBoxLayout *dirButtons = new QHBoxLayout();
    QHBoxLayout *loginLogoutButtons = new QHBoxLayout();
    QHBoxLayout *infoBar = new QHBoxLayout();

    QListView *list = new QListView();
    QLabel* vcbId = new QLabel();
    // QPushButton *button = new QPushButton("Sign In from Google");
    QPushButton *right = new QPushButton(">>");
    QPushButton *left = new QPushButton("<<");
    QPushButton *login = new QPushButton("Login");
    QPushButton *logout = new QPushButton("Logout");
    QPushButton *removeTokens = new QPushButton("Force Logout");
    QStringListModel *sm = new QStringListModel();

    QGuiApplication::setOrganizationName("TestOrgName");
    QGuiApplication::setOrganizationDomain("TestOrgDomainName.com");
    QGuiApplication::setApplicationName("Clipboard Manager");
    QSettings settings("HKEY_CURRENT_USER\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", QSettings::NativeFormat);

    //InfoBar
    QPixmap left_arrow_active_image("./left_arrow_active.png");
    QPixmap left_arrow_inactive_image("./left_arrow_inactive.png");
    QPixmap right_arrow_active_image("./right_arrow_active.png");
    QPixmap right_arrow_inactive_image("./right_arrow_inactive_image.png");
    QPixmap menu_icon("./menu_icon.png");

    QLabel leftArrowInactive;
    leftArrowInactive.setPixmap(left_arrow_inactive_image);
    leftArrowInactive.setStyleSheet("background-color:#2f3233; border: 2px solid gray; border-radius:10px;");
    // leftArrowWidget.setWindowFlags(Qt::Widget | Qt::FramelessWindowHint | Qt::ToolTip | Qt::WindowStaysOnTopHint);
    // leftArrowWidget.setAttribute(Qt::WA_NoSystemBackground, true);
    leftArrowInactive.setFixedSize(30,30);
    leftArrowInactive.setAlignment(Qt::AlignCenter);

    QLabel leftArrowActive;
    leftArrowActive.setPixmap(left_arrow_active_image);
    leftArrowActive.setStyleSheet("background-color:#2f3233; border: 2px solid gray; border-radius:10px;");
    leftArrowActive.setWindowFlags(Qt::Widget | Qt::FramelessWindowHint | Qt::ToolTip | Qt::WindowStaysOnTopHint);
    leftArrowActive.setAttribute(Qt::WA_NoSystemBackground, true);
    leftArrowActive.setFixedSize(30,30);
    leftArrowActive.setAlignment(Qt::AlignCenter);

    QLabel rightArrowActive;
    rightArrowActive.setPixmap(right_arrow_active_image);
    rightArrowActive.setStyleSheet("background-color:#2f3233; border: 2px solid gray; border-radius:10px;");
    rightArrowActive.setWindowFlags(Qt::Widget | Qt::FramelessWindowHint | Qt::ToolTip | Qt::WindowStaysOnTopHint);
    rightArrowActive.setAttribute(Qt::WA_NoSystemBackground, true);
    rightArrowActive.setFixedSize(30,30);
    rightArrowActive.setAlignment(Qt::AlignCenter);

    QLabel rightArrowInactive;
    rightArrowInactive.setPixmap(right_arrow_inactive_image);
    rightArrowInactive.setStyleSheet("background-color:#2f3233; border: 2px solid gray; border-radius:10px;");
    // leftArrowWidget.setWindowFlags(Qt::Widget | Qt::FramelessWindowHint | Qt::ToolTip | Qt::WindowStaysOnTopHint);
    // leftArrowWidget.setAttribute(Qt::WA_NoSystemBackground, true);
    rightArrowInactive.setFixedSize(30,30);
    rightArrowInactive.setAlignment(Qt::AlignCenter);

    QLabel menuIcon;
    menuIcon.setPixmap(menu_icon);
    // menuIcon.setStyleSheet("background-color:#2f3233; border: 2px solid gray; border-radius:10px;");
    menuIcon.setWindowFlags(Qt::Widget | Qt::FramelessWindowHint | Qt::ToolTip | Qt::WindowStaysOnTopHint);
    menuIcon.setAttribute(Qt::WA_NoSystemBackground, true);
    // menuIcon.setFixedSize(30,30);
    menuIcon.setAlignment(Qt::AlignRight);

    QLabel vcbLabel;
    vcbLabel.setText("vcb placeholder");
    vcbLabel.setStyleSheet("background-color:#2f3233; border: 2px solid gray; border-radius:10px;");
    vcbLabel.setFixedWidth(150);
    // vcbLabel.setFixedHeight(30);
    // vcbLabel.setFixedSize(100,30);
    vcbLabel.setAlignment(Qt::AlignCenter);
    // vcbLabel.setAttribute(Qt::WA_TranslucentBackground, true);

    QPalette defaultPalette;
    QBrush base; base.setColor(QColor("#283742"));
    QBrush alternateBase; alternateBase.setColor(QColor("#6784a3"));
    defaultPalette.setBrush(QPalette::Base,base);
    defaultPalette.setBrush(QPalette::AlternateBase,alternateBase);

    const auto fontId = QFontDatabase::addApplicationFont("./resources/Roboto-Medium.ttf");
    QString family = QFontDatabase::applicationFontFamilies(fontId).at(0);
    QFont _font(family, 8);
    a.setFont(_font);

    sm->insertRow(0,sm->index(0));
    sm->setData(sm->index(0),QString("Test Text"));

    list->setModel(sm);
    list->setPalette(defaultPalette);
    list->setAlternatingRowColors(true);
    list->setEditTriggers(QAbstractItemView::NoEditTriggers);
    vcbId->setText("Test VCB");
    
    Handler handler;  

    mainwindow->connect(mainwindow, &MainWindow::registeredShortcutTriggered, &handler, &Handler::handleShortcutTrigger);

    list->connect(list,&QAbstractItemView::doubleClicked,&handler,
        [&](const QModelIndex& ind){
            logger->debug("Double Clicked");
            logger->debug() << list->model()->data(ind).toString();
            // send update to vcbHandler view handler
            handler.doubleClickEvent(ind); 
    });

    handler.connect(&handler,&Handler::updateListViewModel,list,[&](QStringListModel* slm){
        logger->debug("ListViewModel updated");
        list->setModel(slm);
        // logger->debug() << "Slm" << slm->data(slm->index(0)).toString();
    });
    
    handler.connect(&handler,&Handler::updateVcbId,vcbId,[&](QString vcbIdString){
        vcbId->setText(vcbIdString);
    });

    // QObject::connect(&leftArrowActive,&QLabel::clicked,&handler,[&](){
    //     handler.goPrevious();
    // });
    KeyPressEater *keyPressEater = new KeyPressEater();
    leftArrowActive.installEventFilter(keyPressEater);


    right->connect(right,&QPushButton::clicked,&handler,[&](){
        handler.goNext();
    });

    //show the first vcb (hacky way of doing it)
    handler.goPrevious();

    login->connect(login, &QPushButton::clicked, &handler,&Handler::startLogin);
    logout->connect(logout, &QPushButton::clicked, &handler,&Handler::startLogout);
    removeTokens->connect(removeTokens, &QPushButton::clicked, &handler, &Handler::resetAuthTokens);
    dirButtons->addWidget(left);
    dirButtons->addWidget(right);
    loginLogoutButtons->addWidget(login);
    loginLogoutButtons->addWidget(logout);
    loginLogoutButtons->addWidget(removeTokens);
    
    // infoBar->addWidget(&vcbLabelContainer);
    // infoBar->addWidget(&vcbLabelBackground);
    infoBar->addWidget(&leftArrowActive, Qt::AlignCenter);
    infoBar->addWidget(&vcbLabel, Qt::AlignCenter);
    infoBar->addWidget(&rightArrowActive, Qt::AlignCenter);
    infoBar->addWidget(&menuIcon, Qt::AlignLeft);
    // layout->addLayout(dirButtons);
    // layout->addLayout(loginLogoutButtons);
    layout->addLayout(infoBar);
    // layout->addWidget(&vcbLabel);
    layout->addWidget(list);
    layout->addWidget(vcbId);
    
    mainwindow->setStyleSheet("background-color: #283742; color: #aaccff;");
    mainwindow->resize(windowSize.x(), windowSize.y());    

    framelesswindow.setWindowIcon(a.style()->standardIcon(QStyle::SP_DesktopIcon));
    framelesswindow.setWindowTitle("Clippycross");
    framelesswindow.setContent(mainwindow);
    framelesswindow.setPalette(defaultPalette);
    framelesswindow.show();
 
    return a.exec();
}