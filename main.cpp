#include <QApplication>
#include <QListView>
#include <QStringListModel>
#include <QVBoxLayout>
#include <QObject>
#include <QLabel>
#include <QIcon>
#include <QFontDatabase>
#include <QPalette>
#include <QBrush>
#include "framelesswindow/framelesswindow.h"
#include "framelesswindow/DarkStyle.h"
#include <QLoggingCategory>
#include <iostream>
#include <QMenu>
#include <QAbstractItemView>
#include "log4qt/logger.h"
#include "log4qt/loggerrepository.h"
#include "log4qt/consoleappender.h"
#include "log4qt/ttcclayout.h"
#include "log4qt/fileappender.h"
#include "mainwindow.h"
#include "handler.h"
#include "customlistview.h"
#include <QHotkey>
#include <QStandardPaths>
#include <QLineEdit>
#include <QScroller>
#include <QToolTip>

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

int main(int argc, char *argv[])
{
    qInstallMessageHandler(myMessageOutput);
    QApplication a(argc, argv);
    setUpLogger();
    const QPoint windowSize(360,440);
    auto *mainwindow = new MainWindow();
    const auto logoIcon = QIcon(":clippycross_logo.jpg");
    a.setWindowIcon(logoIcon);
    QGuiApplication::setOrganizationName("ClippyCross");
    QGuiApplication::setOrganizationDomain("clippycross.com");
    QGuiApplication::setApplicationName("Clippycross");
    qDebug() << "[main.cpp] main logger->debug Works";
    std::cerr << "This is from standard error" << std::endl;
    std::cout << "This is from standard output" << std::endl; 
    auto logger = Log4Qt::Logger::rootLogger();
    logger->debug() << "This is debug";
    QApplication::setStyle(new DarkStyle);
    FramelessWindow framelesswindow;

    QLabel *tooltipLabel = new QLabel();
    tooltipLabel->setAttribute(Qt::WA_TransparentForMouseEvents);
    tooltipLabel->setStyleSheet("QLabel { background-color : lightyellow; border: 1px solid black; }");
    tooltipLabel->hide();

    QVBoxLayout *layout = new QVBoxLayout(mainwindow);
    QHBoxLayout *infoBar = new QHBoxLayout();

    Handler handler;  
    
//    QListView *list = new QListView();
    auto *list = new CustomListView();
    auto lineEdit = new QLineEdit(mainwindow);
    layout->addWidget(lineEdit, 0,Qt::AlignCenter);
    QPalette defaultPalette;
    QBrush base; base.setColor(QColor("#283742"));
    QBrush alternateBase; alternateBase.setColor(QColor("#6784a3"));
    defaultPalette.setBrush(QPalette::Base,base);
    defaultPalette.setBrush(QPalette::AlternateBase,alternateBase);
    const auto fontId = QFontDatabase::addApplicationFont(":Roboto-Medium.ttf");
    const QString family = QFontDatabase::applicationFontFamilies(fontId).at(0);
    const QFont _font(family, 16);
    QApplication::setFont(_font);

    list->setPalette(defaultPalette);
    list->setAlternatingRowColors(true);
    list->setEditTriggers(QAbstractItemView::NoEditTriggers);
    
    MainWindow::connect(mainwindow, &MainWindow::registeredHotkeyActivated, &handler, &Handler::handleHotkeyActivation);

    QListView::connect(
        list,
        &QAbstractItemView::doubleClicked,
        &handler,
    [&](const QModelIndex& ind){
            logger->debug("Double Clicked");
            logger->debug() << list->model()->data(ind).toString();
            // send update to vcbHandler view handler
            handler.doubleClickEvent(ind);
        }
    );

    QScroller *scroller = QScroller::scroller(list->viewport());
    QScrollerProperties scrollerProperties = scroller->scrollerProperties();
    scrollerProperties.setScrollMetric(QScrollerProperties::VerticalOvershootPolicy, QScrollerProperties::OvershootAlwaysOff);
    scroller->setScrollerProperties(scrollerProperties);

    Handler::connect(&handler,&Handler::updateListViewModel,list,[&](QStringListModel* slm){
        logger->debug("ListViewModel updated");
        list->setModel(slm);
        logger->debug() << "Slm" << slm->data(slm->index(0)).toString();
    });
    list->setModel(handler.getActiveStringListModel());

    QObject::connect(lineEdit, &QLineEdit::returnPressed, &handler,
    [&] {
        logger->debug("Enter clicked on Search Box");
        auto search_query = lineEdit->text();
        handler.onSearchQuery(search_query);
    });

    layout->addLayout(infoBar);
    layout->addWidget(list);

    mainwindow->setWindowTitle("Clippycross");
    mainwindow->setStyleSheet("background-color: #283742; color: #aaccff;");
    mainwindow->resize(windowSize.x(), windowSize.y());    
    auto hotkey = new QHotkey(QKeySequence(Qt::ControlModifier | Qt::ShiftModifier | Qt::Key_C), true, &framelesswindow);//The hotkey will be automatically registered
    qDebug() << "Is HotKey Registered Ctrl + Shift + V  " << hotkey->isRegistered() << " KeyCode : " << hotkey->keyCode();
    QObject::connect(hotkey, &QHotkey::activated, &framelesswindow, [&](){
        qDebug() << "Hotkey Activated - Ctrl + Shift + V";
        framelesswindow.raise();
    });

    framelesswindow.setWindowIcon(logoIcon);
    framelesswindow.setWindowTitle("Clippycross");
    framelesswindow.setContent(mainwindow);
    framelesswindow.setPalette(defaultPalette);
    framelesswindow.show();
 
    return a.exec();
}