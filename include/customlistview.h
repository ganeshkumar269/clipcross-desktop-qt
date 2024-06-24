//
// Created by Ganesh Maddula on 25/05/24.
//

#ifndef CLIPPYCROSS_CUSTOMLISTVIEW_H
#define CLIPPYCROSS_CUSTOMLISTVIEW_H

#include <QListView>
#include <QToolTip>
#include <QMouseEvent>

class CustomListView : public QListView {
    Q_OBJECT

public:
    using QListView::QListView;

protected:
    void mouseMoveEvent(QMouseEvent *event) override {
        QModelIndex index = indexAt(event->pos());
        if (index.isValid()) {
            QString itemText = index.data().toString();
            QPoint tooltipPos = mapToGlobal(event->pos());
            QToolTip::showText(tooltipPos, itemText, this);
        } else {
            QToolTip::hideText();
        }
        QListView::mouseMoveEvent(event);
    }
};

#endif //CLIPPYCROSS_CUSTOMLISTVIEW_H
