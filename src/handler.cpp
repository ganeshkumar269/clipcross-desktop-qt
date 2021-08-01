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

#define VK_1 0x31
#define VK_2 0x32
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
        switch (hotKey) {
            case VK_1:
                qDebug() << "Ctrl + 1 pressed";
                QGuiApplication::clipboard()->setText(vcbHandler->getClipAtIndex(0).value());
                break;
            case VK_2:
                qDebug() << "Ctrl + 2 pressed";
                QGuiApplication::clipboard()->setText(vcbHandler->getClipAtIndex(1).value());
                break;
            default:
                break;
        }


        INPUT ip;
        ip.type = INPUT_KEYBOARD;
        ip.ki.wScan = 0;
        ip.ki.time = 0;
        ip.ki.dwExtraInfo = 0;
        // Press the "Ctrl" key
        ip.ki.wVk = VK_CONTROL;
        ip.ki.dwFlags = 0; // 0 for key press
        SendInput(1, &ip, sizeof(INPUT));

        // Press the "V" key

        ip.ki.wVk = 'V';
        ip.ki.dwFlags = 0; // 0 for key press
        SendInput(1, &ip, sizeof(INPUT));

        // Release the "V" key
        ip.ki.wVk = 'V';
        ip.ki.dwFlags = KEYEVENTF_KEYUP;
        SendInput(1, &ip, sizeof(INPUT));

        // Release the "Ctrl" key
        ip.ki.wVk = VK_CONTROL;
        ip.ki.dwFlags = KEYEVENTF_KEYUP;
        SendInput(1, &ip, sizeof(INPUT));

}