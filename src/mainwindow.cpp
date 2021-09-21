#include <QWidget>
#include "mainwindow.h"
#define WINVER 0x0500
#include <windows.h>
#include <QDebug>
#include <QEvent>
#include <QTimer>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QAction>
#include <QMessageBox>
#include <QCloseEvent>
#include <QHotkey>
#include <QApplication>

MainWindow::MainWindow(QWidget* parent):QWidget(parent){
    qDebug() << "MainWindow Contructor";

    // for(int i = 0x31; i <= 0x39; i++){
    //     if (!RegisterHotKey(HWND(winId()), 0,MOD_CONTROL, i))
    //     {
    //         qDebug() << "Unable to register HotKey Ctrl + " << i - 0x30;
    //     }else {
    //         qDebug() << "Successfully registered Ctrl + " << i - 0x30;
    //     }
    // }
    // for(int i = 1; i <= 9; i++){
        // auto hotkey = new QHotkey(QKeySequence(QString("ctrl+") + QString::number(1)), true, this);//The hotkey will be automatically registered
        // qDebug() << "Is HotKey Registered Ctrl + " << 1 << " : " << hotkey->isRegistered() << " KeyCode : " << hotkey->keyCode();
        // QObject::connect(hotkey, &QHotkey::activated, this, [](){
        //     qDebug() << "Hotkey Activated - Ctrl + 1";
        // });
    // }

    auto hotkey = new QHotkey(QKeySequence(QString("ctrl+") + QString::number(1)), true, this);//The hotkey will be automatically registered
    qDebug() << "Is HotKey Registered Ctrl + " << 1 << " : " << hotkey->isRegistered() << " KeyCode : " << hotkey->keyCode();
    QObject::connect(hotkey, &QHotkey::activated, this, [&](){
        qDebug() << "Hotkey Activated - Ctrl + 1";
        emit registeredHotkeyActivated(QKeySequence(Qt::ControlModifier | Qt::Key_1));
    });

    hotkey = new QHotkey(QKeySequence(QString("ctrl+") + QString::number(2)), true, this);//The hotkey will be automatically registered
    qDebug() << "Is HotKey Registered Ctrl + " << 2 << " : " << hotkey->isRegistered() << " KeyCode : " << hotkey->keyCode();
    QObject::connect(hotkey, &QHotkey::activated, this, [&](){
        qDebug() << "Hotkey Activated - Ctrl + 2";
        emit registeredHotkeyActivated(QKeySequence(Qt::ControlModifier | Qt::Key_2));
    });
 
    hotkey = new QHotkey(QKeySequence(QString("ctrl+") + QString::number(3)), true, this);//The hotkey will be automatically registered
    qDebug() << "Is HotKey Registered Ctrl + " << 3 << " : " << hotkey->isRegistered() << " KeyCode : " << hotkey->keyCode();
    QObject::connect(hotkey, &QHotkey::activated, this, [&](){
        qDebug() << "Hotkey Activated - Ctrl + 3";
        emit registeredHotkeyActivated(QKeySequence(Qt::ControlModifier | Qt::Key_3));
    });

    hotkey = new QHotkey(QKeySequence(QString("ctrl+") + QString::number(4)), true, this);//The hotkey will be automatically registered
    qDebug() << "Is HotKey Registered Ctrl + " << 4 << " : " << hotkey->isRegistered() << " KeyCode : " << hotkey->keyCode();
    QObject::connect(hotkey, &QHotkey::activated, this, [&](){
        qDebug() << "Hotkey Activated - Ctrl + 4";
        emit registeredHotkeyActivated(QKeySequence(Qt::ControlModifier | Qt::Key_4));
    });

    hotkey = new QHotkey(QKeySequence(QString("ctrl+") + QString::number(5)), true, this);//The hotkey will be automatically registered
    qDebug() << "Is HotKey Registered Ctrl + " << 5 << " : " << hotkey->isRegistered() << " KeyCode : " << hotkey->keyCode();
    QObject::connect(hotkey, &QHotkey::activated, this, [&](){
        qDebug() << "Hotkey Activated - Ctrl + 5";
        emit registeredHotkeyActivated(QKeySequence(Qt::ControlModifier | Qt::Key_5));
    });

    hotkey = new QHotkey(QKeySequence(QString("ctrl+") + QString::number(6)), true, this);//The hotkey will be automatically registered
    qDebug() << "Is HotKey Registered Ctrl + " << 6 << " : " << hotkey->isRegistered() << " KeyCode : " << hotkey->keyCode();
    QObject::connect(hotkey, &QHotkey::activated, this, [&](){
        qDebug() << "Hotkey Activated - Ctrl + 6";
        emit registeredHotkeyActivated(QKeySequence(Qt::ControlModifier | Qt::Key_6));
    });

    hotkey = new QHotkey(QKeySequence(QString("ctrl+") + QString::number(7)), true, this);//The hotkey will be automatically registered
    qDebug() << "Is HotKey Registered Ctrl + " << 7 << " : " << hotkey->isRegistered() << " KeyCode : " << hotkey->keyCode();
    QObject::connect(hotkey, &QHotkey::activated, this, [&](){
        qDebug() << "Hotkey Activated - Ctrl + 7";
        emit registeredHotkeyActivated(QKeySequence(Qt::ControlModifier | Qt::Key_7));
    });

    hotkey = new QHotkey(QKeySequence(QString("ctrl+") + QString::number(8)), true, this);//The hotkey will be automatically registered
    qDebug() << "Is HotKey Registered Ctrl + " << 8 << " : " << hotkey->isRegistered() << " KeyCode : " << hotkey->keyCode();
    QObject::connect(hotkey, &QHotkey::activated, this, [&](){
        qDebug() << "Hotkey Activated - Ctrl + 8";
        emit registeredHotkeyActivated(QKeySequence(Qt::ControlModifier | Qt::Key_8));
    });

    hotkey = new QHotkey(QKeySequence(QString("ctrl+") + QString::number(9)), true, this);//The hotkey will be automatically registered
    qDebug() << "Is HotKey Registered Ctrl + " << 9 << " : " << hotkey->isRegistered() << " KeyCode : " << hotkey->keyCode();
    QObject::connect(hotkey, &QHotkey::activated, this, [&](){
        qDebug() << "Hotkey Activated - Ctrl + 9";
        emit registeredHotkeyActivated(QKeySequence(Qt::ControlModifier | Qt::Key_9));
    });
}

bool MainWindow::nativeEvent(const QByteArray &eventType, void *message, qintptr *result){
    Q_UNUSED(eventType);
    Q_UNUSED(result);
    MSG* msg = static_cast<MSG*>(message);

    if (msg->message == WM_HOTKEY)
    {
        qDebug() << "Registered Hotkey triggered";
        emit registeredShortcutTriggered(msg);
        return true;
    }
    return false;
}

// void MainWindow::changeEvent(QEvent* e){
//     switch (e->type())
//     {
//         case QEvent::WindowStateChange:
//             {
//                 if (this->windowState() & Qt::WindowMinimized)
//                 {
//                     qDebug() << "Window minimised";
//                     // if (Preferences::instance().minimizeToTray())
//                     {
//                         QTimer::singleShot(250, this, SLOT(hide()));
//                     }
//                 }

//                 break;
//             }
//         default:
//             break;
//     }
// }

