#include "utils.h"
#include <QCryptographicHash>

QString getMd5Hash(const QString& str){
    QCryptographicHash qh(QCryptographicHash::Md5);
    qh.addData(str.toStdString().c_str(),16);
    return qh.result().toHex();
}