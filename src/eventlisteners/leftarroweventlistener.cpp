#include "leftarroweventlistener.h"
#include <QEvent>
#include <QDebug>
bool LeftArrowEventListener::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::MouseButtonPress) {
        qDebug() << "There is a left arrow press";
        emit clicked(); 
        return true;
    } else {
        // standard event processing
        return QObject::eventFilter(obj, event);
    }
    return true;
}