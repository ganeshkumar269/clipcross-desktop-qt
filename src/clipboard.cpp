#include <QStringListModel>
#include <QClipboard>
#include <QByteArray>
#include <QCryptographicHash>
#include <QApplication>


const int N = 250;


QString formatQString(QString& str){
    return QFontMetricsF(QApplication::font()).elidedText(str,Qt::ElideRight,N);
}


void handleDataChangedEvent(QStringListModel& sm,
                            QClipboard* clipboard,
                            QByteArray& prevItemVal)
{
    
    QString clipboardTextVal = clipboard->text();
    if(prevItemVal != clipboardTextVal){
        qDebug() << "Val doesnt match prevItemVal" << '\n';
        sm.insertRow(0,sm.index(0));
        sm.setData(sm.index(0),formatQString(clipboardTextVal));

        insertItem(clipboardTextVal);

        prevItemVal = clipboardTextVal.toLocal8Bit();
    }
    else{
        qDebug() << "Val matched" << '\n';
    }
}