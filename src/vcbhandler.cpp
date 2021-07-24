#include "vcbhandler.h"
#include <QGuiApplication>
// #include "log4qt/logger.h"
// auto logger = Log4Qt::Logger::rootLogger(); 

VCBHandler::VCBHandler(QObject* parent):QObject(parent){
    vcbList.insert("desktop-one",new VCB("one"));
    vcbList.insert("desktop-two",new VCB("two"));
    vcbList.insert("desktop-three",new VCB("three"));
    vcbList.insert("default-web",new VCB("default_web"));
    visibleVCBId = "desktop-one";
    activeVCBIds.append(visibleVCBId);
    cb = QGuiApplication::clipboard();
    connect(cb,&QClipboard::dataChanged,this,&VCBHandler::onCbDataChanged);
}

VCBHandler::~VCBHandler(){
    QHashIterator<QString, VCB*> i(vcbList);
    while (i.hasNext()) {
        i.next();
        delete i.value();
    }
}

void VCBHandler::add(const Clip& clip,const QString& id){
    if(vcbList.contains(id)){
        vcbList[id]->add(clip);
    }else{
        qDebug() << id << " is not a valid vcbId";
    }
}
void VCBHandler::add(const Clip& clip){
    for(QString id : activeVCBIds)
        add(clip,id);
}
void VCBHandler::add(const Clip& clip,const QStringList& ids){
    for(QString id : ids)
        add(clip,id);
}


Clip VCBHandler::getTopClip(){return vcbList[visibleVCBId]->getTopClip();}
Clip VCBHandler::getTopClip(const QString& id){return vcbList[id]->getTopClip();}
QList<QString> VCBHandler::getActiveVCBIds(){return activeVCBIds;}
QStringListModel* VCBHandler::getModel(){return vcbList[visibleVCBId]->getModel();}
QStringListModel* VCBHandler::getModel(const QString& id){return vcbList[id]->getModel();}
QString VCBHandler::getVisibleVCBId(){return visibleVCBId;}

void VCBHandler::onCbDataChanged(){
    Clip currClip(cb->text(),"text",getTimestamp());
    if( getTopClip().hash() != currClip.hash()){
        qDebug() << "Clip Hash doesnt match prevClipHash" << '\n';
        add(currClip,visibleVCBId);
        emit vcbDataChanged(currClip,{visibleVCBId});
    }
    else{
        qDebug() << "Hash Matched" << '\n';
    }
}

void VCBHandler::next(){
    // auto it = vcbList.find(visibleVCBId);
    QHashIterator<QString, VCB*> i(vcbList);
    while (i.hasNext()) {
        i.next();
        // cout << i.key() << ": " << i.value() << Qt::endl;
        if(i.key() == visibleVCBId) break;
    }
    
    if(i.hasNext()){
        i.next();
        visibleVCBId = i.key(); 
    }
}
void VCBHandler::previous(){
    // auto it = vcbList.find(visibleVCBId);
    QHashIterator<QString, VCB*> i(vcbList);
    QString prevVal = visibleVCBId;
    while (i.hasNext()) {
        i.next();
        // cout << i.key() << ": " << i.value() << Qt::endl;
        if(i.key() == visibleVCBId) break;
        prevVal = i.key();
    }
    visibleVCBId = prevVal;
}

QList<QPair<QString,Clip>> VCBHandler::vcbTopClips(){
    QList<QPair<QString,Clip>> data;
    data.append({"desktop-one", getTopClip("desktop-one")});
    data.append({"desktop-two", getTopClip("desktop-two")});
    data.append({"desktop-three", getTopClip("desktop-three")});
    return data;
}