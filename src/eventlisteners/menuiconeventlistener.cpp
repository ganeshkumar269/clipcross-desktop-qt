#include "menuiconeventlistener.h"
#include <QEvent>
#include <QDebug>
bool MenuIconEventListener::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::MouseButtonPress) {
        qDebug() << "There is a menu icon press";
        emit clicked(); 
        return true;
    } else {
        // standard event processing
        return QObject::eventFilter(obj, event);
    }
    return true;
}