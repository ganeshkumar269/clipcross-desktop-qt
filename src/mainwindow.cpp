#include <QWidget>
#include "mainwindow.h"
#define WINVER 0x0500
#include <windows.h>
#include <QDebug>
MainWindow::MainWindow(QWidget* parent):QWidget(parent){
    qDebug() << "MainWindow Contructor";

    for(int i = 0x31; i <= 0x39; i++){
        if (!RegisterHotKey(HWND(winId()), 0,MOD_CONTROL, i))
        {
            qDebug() << "Unable to register HotKey Ctrl + " << i - 0x30;
        }else {
            qDebug() << "Successfully registered Ctrl + " << i - 0x30;
        }
    }
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