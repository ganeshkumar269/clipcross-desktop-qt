######################################################################
# Automatically generated by qmake (3.1) Mon Apr 5 13:24:08 2021
QT       += core gui sql network websockets widgets concurrent
TEMPLATE = app
TARGET = clippycross

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

include(vendor/vendor.pri)

RESOURCES += framelesswindow.qrc darkstyle.qrc clippycross.qrc

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
           include\rightarroweventlistener.h \
           include\leftarroweventlistener.h \
           include\menuiconeventlistener.h \
           include\prefs.h \
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
           src/eventlisteners/rightarroweventlistener.cpp \
           src/eventlisteners/leftarroweventlistener.cpp \
           src/eventlisteners/menuiconeventlistener.cpp \
           src/mainwindow.cpp \
           src/prefs.cpp \
           #debug/qrc_darkstyle.cpp \
