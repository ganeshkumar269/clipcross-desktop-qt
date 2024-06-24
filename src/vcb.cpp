#include "vcb.h"
VCB::VCB(const QString& id,const QString& deviceId, QObject* parent)
    :id(id),deviceId(deviceId),QObject(parent)
{
    slm = new QStringListModel();
    //show the first item as the vcbId
//    slm->insertRow(0,slm->index(0));
//    slm->setData(slm->index(0),id);
    clips = new QList<Clip>();
    db = new Database(id);
    clips = db->retrieveClips({});
    addClipsToSlm(clips);
    topClip = !clips->empty() ? clips->at(0) : Clip("Empty Database","text",0);
    qDebug() << "VcbId: " << id << " topclip: " << topClip.toString();
}
VCB::~VCB(){
    delete slm;
    delete clips;
    if(db != nullptr){
        delete db;
    }
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
    const Clip clip = clips->at(index.row());
    if(db->checkIfHashIsPresent(clip.hash())){
        // update timestamp of the text if its already present in db, so when we fetch its comes in timestamp sorted order
        // and remove from the StringListModel
//        db->updateTimestampOfClip(clip.hash(), getTimestamp());
        removeFromSlm(index.row());
    }
    add(clip);
}

void VCB::checkForDuplicateAndAdd(const Clip& clip){
    for(int i = 0; i < clips->size(); i++)
        if(clips->at(i).hash() == clip.hash()){
            removeFromSlm(i);
            break;
        }
    add(clip);
}

void VCB::removeFromSlm(int index){
//    if(!nonHost){
//        db->deleteClip(clip.hash());
//    }
    clips->removeAt(index);
    slm->removeRow(index);
}

Clip VCB::getTopClip(){return topClip;}
QStringListModel* VCB::getModel(){return slm;}
Clip VCB::getClipAtIndex(int index){
    if(index < 0 || index >= clips->size()){
        qDebug() << "index " << index << " is out of range";
        return clips->at(0); // out of range just send the first clip
    }else{
        return clips->at(index);
    }
}
QList<Clip>* VCB::onSearchQuery(const QString& searchQuery) {
    return db->onSearchQuery(searchQuery);
}
