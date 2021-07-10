#include "utils.h"
#include <QDateTime>

qint64 getTimestamp(){return QDateTime::currentMSecsSinceEpoch();}