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
    emit updateListViewModel(vcbHandler->getModel());
    authenticate = new Authenticate();
    authenticate->connect(authenticate,&Authenticate::authenticated,this,[&](){
        qDebug() << "Authentication Success" ;
        QSettings s;
        qDebug() << s.value("id_token").toString();
        qDebug() << s.value("refresh_token").toString();
        qDebug() << s.value("device_id").toString();
        initWsw();
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
    connect(wsw,&WebSocketW::clipReceived,this,&Handler::onWswClipReceived);
    connect(wsw,&WebSocketW::wssTokenExpired,this,&Handler::onWssTokenExpired);
    connect(wsw,&WebSocketW::syncFlowDataReceived,this,&Handler::syncOperation);
}


void Handler::onWssTokenExpired(){
    authenticate->refreshAccessToken();
    authenticate->connect(authenticate,&Authenticate::accessTokenRefreshed,
        this,[&](){
            initWsw();
    });
}

void Handler::syncOperation(const QJsonObject& payload){
    qDebug() << __FILE__ << __FUNCTION__;
    if(!payload.empty())
    for(auto id : payload.keys()){
        qDebug() << "id: " << id;
        Clip clip(
                    payload[id]["value"].toString(),
                    payload[id]["format"].toString(),
                    payload[id]["timestamp"].toInteger()
                );

        if(clip.timestamp() > vcbHandler->getTopClip(id).timestamp()){
            //client lags the server
            qDebug() << "Client lags Server";
            vcbHandler->add(clip,id);
        }
        else if(clip.timestamp() < vcbHandler->getTopClip(id).timestamp()){
            qDebug() << "Server lags Client";
            wsw->sendClip(vcbHandler->getTopClip(id),{id});
        }
        else {
            qDebug() << "Server and Client are in sync";
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