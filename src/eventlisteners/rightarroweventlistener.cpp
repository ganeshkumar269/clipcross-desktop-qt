#include "rightarroweventlistener.h"
#include <QEvent>
#include <QDebug>
bool RightArrowEventListener::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::MouseButtonPress) {
        qDebug() << "There is a right arrow press";
        emit clicked(); 
        return true;
    } else {
        // standard event processing
        return QObject::eventFilter(obj, event);
    }
    return true;
}