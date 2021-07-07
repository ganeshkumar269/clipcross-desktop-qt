#include "utils.h"
#include <QString>
#include <QApplication>
#include <QFontMetricsF>
QString formatQString(const QString& str){
    std::string no_newlines = str.toStdString();
    for(char &i : no_newlines)if(i == '\n')i = ' ';
    return QFontMetricsF(QApplication::font()).elidedText(no_newlines.c_str(),Qt::ElideRight,250);
}