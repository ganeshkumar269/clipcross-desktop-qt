#include <QWidget>
#include "mainwindow.h"
#define WINVER 0x0500
#include <windows.h>
#include <QDebug>
MainWindow::MainWindow(QWidget* parent):QWidget(parent){
    qDebug() << "MainWindow Contructor";
}

bool MainWindow::nativeEvent(const QByteArray &eventType, void *message, qintptr *result){
    Q_UNUSED(eventType);
    Q_UNUSED(result);
    MSG* msg = static_cast<MSG*>(message);

    if (msg->message == WM_HOTKEY)
    {
        qDebug() << "There is a key press";
        emit configuredShortcutTriggered(msg);
        return true;
    }
    return false;
}