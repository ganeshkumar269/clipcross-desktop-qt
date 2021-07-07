#include "vcb.h"
VCB::VCB(const QString& id,QObject* parent):id(id),QObject(parent)
{
    slm = new QStringListModel();
    
    slm->insertRow(0,slm->index(0));
    slm->setData(slm->index(0),id);
    
    clips = new QList<Clip>();
    db = new Database(id);
    clips = db->retrieveClips({});
    addClipsToSlm(clips);
    topClip = clips->size() > 0 ? clips->at(0) : Clip("","",0);
    qDebug() << __FILE__ << __FUNCTION__ << " topclip: " << topClip.toString();
    //lastSyncedClip
}
VCB::~VCB(){
    delete slm;
    delete clips;
    delete db;
}
void VCB::addClipToSlm(const Clip& clip)
{
    slm->insertRow(0,slm->index(0));
    slm->setData(slm->index(0),formatQString(clip.value()));
    // db->insertClip(clip);
}

void VCB::addClipsToSlm(const QList<Clip>* list){
    if(list == nullptr) return;
    for(int i = list->size() - 1; i >= 0;i--)
        addClipToSlm(list->at(i));
    // if(list->size() > 0)
    //     updatePrevClip(list->at(0));
}

void VCB::add(const Clip& clip){
    clips->insert(0,clip); //check for optimisation ie insert vs append
    addClipToSlm(clip);
    db->insertClip(clip);
    topClip = clip;
}

void VCB::add(const QModelIndex& index){
    // Clip clip(slm->data(index),clips->at(index.row()),);
    clips->insert(0,clips->at(index.row()));
    // clips->insert(0,slm->data(index));
    addClipToSlm(clips->at(index.row()));
    topClip = clips->at(index.row());
}
Clip VCB::getTopClip(){return topClip;}
QStringListModel* VCB::getModel(){return slm;}