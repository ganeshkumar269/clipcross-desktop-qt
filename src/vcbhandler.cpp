#include "vcbhandler.h"
#include <QGuiApplication>

VCBHandler::VCBHandler(QObject* parent):QObject(parent){
    vcbList.insert("one",new VCB("one"));
    vcbList.insert("two",new VCB("two"));
    vcbList.insert("three",new VCB("three"));
    visibleVCBId = "one";
    // topClip = vcbList[visibleVCBId]->getTopClip();
    // activeVCBIds = new QList<QString>();
    activeVCBIds.append(visibleVCBId);
    
    cb = QGuiApplication::clipboard();
    connect(cb,&QClipboard::dataChanged,this,&VCBHandler::onCbDataChanged);
}

VCBHandler::~VCBHandler(){
    // delete activeVCBIds;
    QHashIterator<QString, VCB*> i(vcbList);
    while (i.hasNext()) {
        i.next();
        // cout << i.key() << ": " << i.value() << Qt::endl;
        delete i.value();
    }
}

void VCBHandler::add(const Clip& clip,const QString& id){
    vcbList[id]->add(clip);
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
    if( getTopClip().hash() != currClip.hash())//format assumed text
    {
        qDebug() << "Clip Hash doesnt match prevClipHash" << '\n';
        add(currClip,visibleVCBId);
        // add(currClip);
        emit vcbDataChanged(currClip,{visibleVCBId});
    }
    else
    {
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