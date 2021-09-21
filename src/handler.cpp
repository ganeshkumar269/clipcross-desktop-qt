#include "handler.h"
#include <QGuiApplication>
#include <QtWebSockets/QWebSocket>
#include <QTimer>
#include "utils.h"
#include "clip.h"
#include "vcbhandler.h"
#include <QClipboard>
#define WINVER 0x0500
#include <windows.h>
#include <QKeySequence>
QT_USE_NAMESPACE

Handler::Handler(QObject* parent):QObject(parent)
{
    qDebug() << "Handler Constructor"; 
    wsw = nullptr;
    vcbHandler = new VCBHandler();
    connect(vcbHandler,&VCBHandler::vcbDataChanged,this,&Handler::onVCBDataChanged);
    authenticate = new Authenticate();
    authenticate->connect(authenticate,&Authenticate::authenticated,this,[&](){
        qDebug() << "Authentication Success" ;
        QSettings s;
        qDebug() << s.value("id_token").toString();
        qDebug() << s.value("refresh_token").toString();
        qDebug() << s.value("device_id").toString();
        initWsw();
        authenticate->stopAuthServer();
    });
}

Handler::~Handler()
{
    if(wsw != nullptr) delete wsw;
    if(vcbHandler != nullptr) delete vcbHandler;
}

void Handler::onVCBDataChanged(const Clip& clip,const QList<QString>& ids)
{
    if(wsw != nullptr){
        wsw->sendClip(clip,ids);
    }
}


void Handler::onWswClipReceived(const Clip& clip,const QStringList& ids){
    qDebug() << "[handler.cpp] onWswClipReceived";
    vcbHandler->add(clip,ids);
}

void Handler::initWsw(){
    if(wsw != nullptr) delete wsw;
    wsw = new WebSocketW();
    // connect(wsw,&WebSocketW::getTopClips, this, &Handler::handleGetTopClips)
    connect(wsw,&WebSocketW::clipReceived,this,&Handler::onWswClipReceived);
    connect(wsw,&WebSocketW::wssTokenExpired,this,&Handler::onWssTokenExpired);
    connect(wsw,&WebSocketW::syncFlowDataReceived,this,&Handler::syncOperation);
    connect(wsw,&WebSocketW::wssConnectionEstablished,this,&Handler::onWssConnectionEstablished);
    connect(wsw,&WebSocketW::wssReadyToTransferData,this,&Handler::onWssReadyToTransferData);
}


void Handler::onWssTokenExpired(){
    authenticate->refreshAccessToken();
    authenticate->connect(authenticate,&Authenticate::accessTokenRefreshed,
        this,[&](){
            initWsw();
    });
}

void Handler::syncOperation(const QJsonObject& payload){
    qDebug() << "handling syncflow response";
    if(!payload.empty())
    if(!payload.contains("clips")){
        qDebug() << "Syncflow Response object doesnot have clips";
        return;
    }
    const QJsonObject& clips = payload["clips"].toObject();
    for(const auto deviceId : clips.keys()){
        qDebug() << "deviceId: " << deviceId;

        for(const auto vcbId : clips[deviceId].toObject().keys()){
            qDebug() << "vcbId: " << vcbId;
            if(vcbHandler->hasVcbId(vcbId)){
                qDebug() << vcbId << " says: \"Server can't poke in my vcbs\"";
                continue;
            }

            Clip clip(
                        clips[deviceId][vcbId]["value"].toString(),
                        clips[deviceId][vcbId]["format"].toString(),
                        clips[deviceId][vcbId]["timestamp"].toInteger()
                    );
            qDebug() << "Recieved Clip for the vcbId: " << vcbId << " clip:" << clip.toString();
            vcbHandler->add(clip,vcbId,deviceId);
        }
    }
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
void Handler::startAuthentication(){
    authenticate->start();
}
void Handler::startLogout(){
    authenticate->logout();
}
void Handler::startLogin(){
    authenticate->login();
}
void Handler::onWssConnectionEstablished(){
    // wsw->initSyncFlow(vcbHandler->vcbTopClips());
}
void Handler::onWssReadyToTransferData(){
    wsw->initSyncFlow(vcbHandler->vcbTopClips());
}
void Handler::resetAuthTokens(){
    QSettings s;
    s.remove("refresh_token");
    s.remove("id_token");
    s.remove("device_id");
}

void Handler::handleShortcutTrigger(MSG* msg){
    WORD hotKey =  HIWORD(msg->lParam);
    int numberTriggered = hotKey - 0x30;
    if(numberTriggered < 1 || numberTriggered > 9){
        qDebug() << "Hotkey : " << hotKey << " is invalid";
        return;
    }
    qDebug() << "Ctrl + " <<  numberTriggered << " pressed";
    vcbHandler->setHandleClipboardUpdates(false);
    QString topClipValue = vcbHandler->getTopClip().value();
    QGuiApplication::clipboard()->setText(vcbHandler->getClipAtIndex(numberTriggered-1).value());
    simulatePasteKeySequence();
    QTimer::singleShot(1000,[=](){
        QGuiApplication::clipboard()->setText(topClipValue);
        vcbHandler->setHandleClipboardUpdates(true);
    });
}

void Handler::doubleClickEvent(const QModelIndex& index){
    vcbHandler->handleDoubleClickEvent(index);
}

void Handler::handleHotkeyActivation(const QKeySequence& keyseq){
    if(keyseq == QKeySequence(Qt::ControlModifier | Qt::Key_1)){
        qDebug() << " Ctrl + 1 captured";
        changeClipboardTemporarily(vcbHandler->getClipAtIndex(0));
    }
    else if(keyseq == QKeySequence(Qt::ControlModifier | Qt::Key_2)){
        qDebug() << " Ctrl + 2 captured";
        changeClipboardTemporarily(vcbHandler->getClipAtIndex(1));
    }
    else if(keyseq == QKeySequence(Qt::ControlModifier | Qt::Key_3)){
        qDebug() << " Ctrl + 3 captured";
        changeClipboardTemporarily(vcbHandler->getClipAtIndex(2));
    }
    else if(keyseq == QKeySequence(Qt::ControlModifier | Qt::Key_4)){
        qDebug() << " Ctrl + 4 captured";
        changeClipboardTemporarily(vcbHandler->getClipAtIndex(3));
    }
    else if(keyseq == QKeySequence(Qt::ControlModifier | Qt::Key_5)){
        qDebug() << " Ctrl + 5 captured";
        changeClipboardTemporarily(vcbHandler->getClipAtIndex(4));
    }
    else if(keyseq == QKeySequence(Qt::ControlModifier | Qt::Key_6)){
        qDebug() << " Ctrl + 6 captured";
        changeClipboardTemporarily(vcbHandler->getClipAtIndex(5));
    }
    else if(keyseq == QKeySequence(Qt::ControlModifier | Qt::Key_7)){
        qDebug() << " Ctrl + 7 captured";
        changeClipboardTemporarily(vcbHandler->getClipAtIndex(6));
    }
    else if(keyseq == QKeySequence(Qt::ControlModifier | Qt::Key_8)){
        qDebug() << " Ctrl + 8 captured";
        changeClipboardTemporarily(vcbHandler->getClipAtIndex(7));
    }
    else if(keyseq == QKeySequence(Qt::ControlModifier | Qt::Key_9)){
        qDebug() << " Ctrl + 9 captured";
        changeClipboardTemporarily(vcbHandler->getClipAtIndex(8));
    }else {
        qDebug() << "Hotkey "  << keyseq << " not handled";
    }
}

void Handler::changeClipboardTemporarily(const Clip& clip){
    vcbHandler->setHandleClipboardUpdates(false);
    QString topClipValue = vcbHandler->getTopClip().value();
    QGuiApplication::clipboard()->setText(clip.value());
    simulatePasteKeySequence();
    QTimer::singleShot(1000,[=](){
        QGuiApplication::clipboard()->setText(topClipValue);
        vcbHandler->setHandleClipboardUpdates(true);
    });
}