#include "prefs.h"
#include <QSettings>
#include <QCoreApplication>
#include <QStandardPaths>

bool getStartupPref(){
    QSettings sets;
    if(sets.contains("ClippyCross:Preferences:Run_At_Startup")){
        return sets.value("ClippyCross:Preferences:Run_At_Startup").toBool();
    }
    return false;
}

void setStartupPref(bool flag){
    QSettings sets;
    sets.setValue("ClippyCross:Preferences:Run_At_Startup", flag);
}

void runAppAtStartup(){
    QSettings settings("HKEY_CURRENT_USER\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", QSettings::NativeFormat);
    QString value = QCoreApplication::applicationFilePath();
    QString apostroph = "\"";
    value.replace("/","\\");
    value = apostroph + value + apostroph + " --argument";
    qDebug() << "Value adding to registry " << value;
    settings.setValue("clippycross-startup-setup", value);
}

void undoRunAppAtStartup(){
    QSettings settings("HKEY_CURRENT_USER\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", QSettings::NativeFormat);
    settings.remove("clippycross-startup-setup");
} 