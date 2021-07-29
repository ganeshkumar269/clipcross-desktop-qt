#include "handler.h"
#include <QGuiApplication>
#include <QtWebSockets/QWebSocket>
#include <QTimer>
#include "utils.h"
#include "clip.h"
#include "vcbhandler.h"
QT_USE_NAMESPACE
// #include "log4qt/logger.h"
// auto logger = Log4Qt::Logger::rootLogger(); 

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