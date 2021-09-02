######################################################################
# Automatically generated by qmake (3.1) Mon Apr 5 13:24:08 2021
######################################################################
QT       += core gui sql network websockets widgets concurrent
TEMPLATE = app
TARGET = tryhard
INCLUDEPATH += .
INCLUDEPATH += ./include/
INCLUDEPATH += ./dependencies/
LIBS += -L./debug/ -llog4qt
LIBS += -L./dependencies/qhttp/ -lqhttp
LIBS += -luser32
CONFIG += debug
OUTPUT += Console
DEFINES += LOCAL_WSS_TEST 
DEFINES += LOG4QT_LIBRARY
DEFINES += QHTTP_EXPORT
QMAKE_CXXFLAGS += -fpermissive

# You can make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# Please consult the documentation of the deprecated API in order to know
# how to port your code away from it.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
# DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

# Input

RESOURCES = framelesswindow.qrc darkstyle.qrc 

FORMS += dependencies\framelesswindow\framelesswindow.ui \


HEADERS += include\database.h \
           include\handler.h \
           include\utils.h \
           include\websocketw.h \
           include\authenticate.h \
           include\clip.h \
           include\vcb.h \
           include\vcbhandler.h \
           include\constants.h \
           include\mainwindow.h \
           dependencies\framelesswindow\framelesswindow.h \
           dependencies\framelesswindow\windowdragger.h \
           dependencies\framelesswindow\DarkStyle.h \


SOURCES += main.cpp \
           $$files(src/database/*.cpp) \
           $$files(src/utils/*.cpp) \
           src/getWssUrl.cpp \
           src/handler.cpp \
           src/websocketw.cpp \
           src/authenticate.cpp \
           src/vcb.cpp \
           src/getQueryParamValue.cpp \
           src/vcbhandler.cpp \
           dependencies\framelesswindow\framelesswindow.cpp \
           dependencies\framelesswindow\windowdragger.cpp \
           dependencies\framelesswindow\DarkStyle.cpp \
           src/mainwindow.cpp \
           #debug/qrc_darkstyle.cpp \
