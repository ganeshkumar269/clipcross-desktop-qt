#include "utils.h"
#include <QString>

QString& quotedQString(QString&str){
    str.prepend("\"");
    str.append("\"");
    return str;
}