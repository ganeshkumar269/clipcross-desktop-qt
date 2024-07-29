#include "handler.h"
#include <QGuiApplication>
#include <QTimer>
#include "utils.h"
#include "clip.h"
#include "vcbhandler.h"
#include <QKeySequence>
QT_USE_NAMESPACE

Handler::Handler(QObject* parent):QObject(parent)
{
    qDebug() << "Handler Constructor"; 
    vcbHandler = new VCBHandler();
    connect(vcbHandler,&VCBHandler::vcbDataChanged,this,&Handler::onVCBDataChanged);
}

Handler::~Handler()
{
    // if(wsw != nullptr) delete wsw;
    delete vcbHandler;
}

void Handler::onVCBDataChanged(const Clip& clip,const QList<QString>& ids)
{
    // if(wsw != nullptr){
    //     wsw->sendClip(clip,ids);
    // }
}

void Handler::goNext(){
    vcbHandler->next();
    emit updateListViewModel(vcbHandler->getModel());
    emit updateVcbId(vcbHandler->getVisibleVCBId());
}
void Handler::goPrevious(){
    vcbHandler->previous();
    emit updateListViewModel(vcbHandler->getModel());
    emit updateVcbId(vcbHandler->getVisibleVCBId());
}

void Handler::doubleClickEvent(const QModelIndex& index){
    vcbHandler->handleDoubleClickEvent(index);
}

void Handler::handleHotkeyActivation(const QKeySequence& keyseq){
    if(keyseq == QKeySequence(Qt::ControlModifier | Qt::ShiftModifier | Qt::Key_C)){
        qDebug() << " Ctrl + Shift + C captured";
        // changeClipboardTemporarily(vcbHandler->getClipAtIndex(0));
    }
}

void Handler::changeClipboardTemporarily(const Clip& clip){
    vcbHandler->setHandleClipboardUpdates(false);
    QString topClipValue = vcbHandler->getTopClip().value();
    QGuiApplication::clipboard()->setText(clip.value());
    // simulatePasteKeySequence();
    QTimer::singleShot(1000,[=](){
        QGuiApplication::clipboard()->setText(topClipValue);
        vcbHandler->setHandleClipboardUpdates(true);
    });
}

QStringListModel* Handler::getActiveStringListModel() {
    return vcbHandler->getModel();
}


void Handler::clearSearchQueryResultAndShowVCB(){
    if(vcbHandler->getSearchQueryResultSLM() != nullptr){
        vcbHandler->clearSearchQueryResultData();
    }
    emit updateListViewModel(vcbHandler->getModel());
}

void Handler::onSearchQuery(const QString& search_query) {
    if(search_query.trimmed().size() == 0){
        clearSearchQueryResultAndShowVCB();
    }else{
        auto searchQueryResultSLM =  vcbHandler->onSearchQuery(search_query);
        emit updateListViewModel(searchQueryResultSLM);
    }
}

void Handler::onClearSearchButton() {
    clearSearchQueryResultAndShowVCB();
}
