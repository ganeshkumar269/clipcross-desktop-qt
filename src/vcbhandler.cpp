#include "vcbhandler.h"
#include <QGuiApplication>
#include <QMimeData>
#include  <QTimer>
#include "utils.h"
// #include "log4qt/logger.h"
// auto logger = Log4Qt::Logger::rootLogger(); 

VCBHandler::VCBHandler(QObject* parent):QObject(parent){
    vcbList.insert("desktop-one",new VCB("one", "desktop"));
    // vcbList.insert("desktop-two",new VCB("two","desktop"));
    // vcbList.insert("desktop-three",new VCB("three","desktop"));
    visibleVCBId = "desktop-one";
    visibleVCBIdIndex = 0;
    handleClipboardUpdates = true;
    activeVCBIds.append(visibleVCBId);
    vcbListOrder = new QStringList();
    vcbListOrder->append("desktop-one");
    cb = QGuiApplication::clipboard();
    // connect(cb,&QClipboard::dataChanged,this,&VCBHandler::onCbDataChanged);
    clipboardTimer = new QTimer(this);
    connect(clipboardTimer, &QTimer::timeout, this, &VCBHandler::checkClipboardChanged);
    clipboardTimer->start(500); // Adjust interval as needed (in milliseconds)
}

VCBHandler::~VCBHandler(){
    QHashIterator<QString, VCB*> i(vcbList);
    while (i.hasNext()) {
        i.next();
        delete i.value();
    }
}
void VCBHandler::checkClipboardChanged() {
    onCbDataChanged();
}

void VCBHandler::add(const Clip& clip,const QString& id, const QString& deviceId){
    if(vcbList.contains(id)){
        vcbList[id]->checkForDuplicateAndAdd(clip);
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
QString VCBHandler::getVisibleVCBId(){return vcbListOrder->at(visibleVCBIdIndex);}

void VCBHandler::onCbDataChanged(){
    qDebug() << "Clipboard Update Happened handleClipboardUpdate: " << handleClipboardUpdates;
    const QString format = cb->mimeData()->formats().contains("text/plain") ? "text" : "file";
    qDebug() << "Format of data in clipboard " << format;

    if(!handleClipboardUpdates){
        qDebug() << "Clipboard update happened, but handleClipboardUpdates is set to false";
        return;
    }
    Clip currClip(cb->text(), format, getTimestamp());
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
    if(visibleVCBIdIndex < vcbList.size() - 1){
        visibleVCBIdIndex++;
        visibleVCBId = vcbListOrder->at(visibleVCBIdIndex);
    }else{
        qDebug() << "VcbList end reached";
    }

}
void VCBHandler::previous(){
    if(visibleVCBIdIndex > 0){
        visibleVCBIdIndex--;
        visibleVCBId = vcbListOrder->at(visibleVCBIdIndex);
    }else{
        qDebug() << "VcbList beginning reached";
    }
}

QList<QPair<QString,Clip>> VCBHandler::vcbTopClips(){
    QList<QPair<QString,Clip>> data;
    data.append({"desktop-one", getTopClip("desktop-one")});
    data.append({"desktop-two", getTopClip("desktop-two")});
    data.append({"desktop-three", getTopClip("desktop-three")});
    return data;
}

bool VCBHandler::hasVcbId(const QString& id){
    return vcbList.contains(id);
}

Clip VCBHandler::getClipAtIndex(int index){
    return vcbList[visibleVCBId]->getClipAtIndex(index);
}
void VCBHandler::setHandleClipboardUpdates(bool flag){
    qDebug() << "Setting hanldeClipboardUpdate: " << flag;
    handleClipboardUpdates = flag;
}

void VCBHandler::handleDoubleClickEvent(const QModelIndex& index){
    // consider condition if the active SLM being a search query result slm
    if(getSearchQueryResultSLM() != nullptr){
        auto clip = searchQueryResultClips->at(index.row());
//        QString clipText = getSearchQueryResultSLM()->data(index).toString();
//        Clip clip = Clip(clipText, "text", getTimestamp());
        vcbList[visibleVCBId]->add(clip);
        cb->setText(vcbList[visibleVCBId]->getTopClip().value());
    }else{
        vcbList[visibleVCBId]->add(index);
        cb->setText(vcbList[visibleVCBId]->getTopClip().value());
    }
}

QStringListModel* VCBHandler::onSearchQuery(const QString& search_query) {
//    QStringListModel* searchQueryResultSLM = vcbList[visibleVCBId]->onSearchQuery(search_query);
    auto list = vcbList[visibleVCBId]->onSearchQuery(search_query);
    const auto pQStringListModel = new QStringListModel();
    for(auto i = 0; i < list->size(); i++){
        auto clip = list->at(i);
        pQStringListModel->insertRow(i, pQStringListModel->index(i));
        pQStringListModel->setData(pQStringListModel->index(i), formatQString(clip.value()));
    }
    // make sure to clear any existing search result slm
    if(getSearchQueryResultSLM() != nullptr){
        clearSearchQueryResultData();
    }
    // store the SLM for later deletion
    searchQueryResultClips = list;
    setSearchQueryResultSLM(pQStringListModel);
    return searchQueryResultSLM;
}

void VCBHandler::setSearchQueryResultSLM(QStringListModel * slm) {
    this->searchQueryResultSLM = slm;
}

QStringListModel *VCBHandler::getSearchQueryResultSLM() {
    return this->searchQueryResultSLM;
}
void VCBHandler::clearSearchQueryResultData(){
    delete this->searchQueryResultSLM;
    this->searchQueryResultSLM = nullptr;
    delete this->searchQueryResultClips;
    this->searchQueryResultClips = nullptr;
}
