#include "handler.h"
#include <QGuiApplication>
#include <QtWebSockets/QWebSocket>
#include <QTimer>
#include "utils.h"
#include "clip.h"
#include "vcbhandler.h"
QT_USE_NAMESPACE

Handler::Handler(QObject* parent):QObject(parent)
{
    qDebug() << "Handler Constructor"; 
    authenticate = new Authenticate();
    authenticate->connect(authenticate,&Authenticate::authenticated,this,[&](){
        qDebug() << "Authentication Success" ;
        QSettings s;
        qDebug() << s.value("access_token");
        qDebug() << s.value("refresh_token");
        qDebug() << s.value("device_id");
        // button->setVisible(false);
        // list->setVisible(true);
        // handler = new Handler(nullptr);
        // handler->setSM(sm);
        // db = new Database();
        // cb = QGuiApplication::clipboard();
        vcbHandler = new VCBHandler();
        wsw = nullptr;
        connect(vcbHandler,&VCBHandler::vcbDataChanged,this,&Handler::onVCBDataChanged);
        emit updateListViewModel(vcbHandler->getModel());
        // prevItemVal = new QByteArray("TempValue");
        // prevClip = new Clip("","",0);

        //clipboard signals
        // connect(cb,QClipboard::dataChanged,this,&Handler::onCBDataChanged);
        //wsw
        // connect(wsw,&WebSocketW::wswDisconnected,)
        initWsw();
    });
    authenticate->start();
}

Handler::~Handler()
{
    // if(db != nullptr) delete db;
    if(wsw != nullptr) delete wsw;
    if(vcbHandler != nullptr) delete vcbHandler;
}

void Handler::onVCBDataChanged(const Clip& clip,const QList<QString>& ids)
{
    // for(QString id : ids)
    wsw->sendClip(clip,ids);
}

// void Handler::setSM(QStringListModel*const m)
// {
//     sm = m;
//     addClipsToSM(db->retrieveClips("dbv2",{"value","format"}));

// }

// void Handler::addClipToSM(const Clip& clip)
// {
//     sm->insertRow(0,sm->index(0));
//     sm->setData(sm->index(0),clip.value());
// }

// void Handler::addClipsToSM(const QList<Clip>* list){
//     if(list == nullptr) return;
//     for(int i = list->size() - 1; i >= 0;i--)
//         addClipToSM(list->at(i));
//     if(list->size() > 0)
//         updatePrevClip(list->at(0));
// }

void Handler::onWswClipReceived(const Clip& clip,const QStringList& ids){
    qDebug() << "[handler.cpp] onWswClipReceived";
    // if(db->insertClip(clip)){
    //     addClipToSM(clip);
    // }

    vcbHandler->add(clip,ids);
}
// void Handler::updatePrevClip(const Clip& clip){
//     qDebug() << "handler.cpp updatePrevClipHash ";
//     if(prevClip != nullptr) delete prevClip;
//     prevClip = new Clip(clip); 
//     qDebug() << __FILE__ << __FUNCTION__ << "updated prevClip" << prevClip;
// }
void Handler::initWsw(){
    if(wsw != nullptr) delete wsw;
    wsw = new WebSocketW();
    connect(wsw,&WebSocketW::clipReceived,this,&Handler::onWswClipReceived);
    connect(wsw,&WebSocketW::wssTokenExpired,this,&Handler::onWssTokenExpired);
    connect(wsw,&WebSocketW::syncFlowDataReceived,this,&Handler::syncOperation);
    // int delay = 0;
    // if(wsw) {
    //     delay=1500;
    //     delete wsw;
    //     QTimer::singleShot(delay,this,[&](){
    //         wsw = new WebSocketW(); 
    //         connect(wsw,&WebSocketW::wswDisconnected,this,&Handler::initWsw);
    //         connect(wsw,&WebSocketW::messageReceived,this,&Handler::onWswMessageReceived);
    //     });
    // }else {
    //     wsw_retries = 0;
    //     wsw = new WebSocketW();
    //     connect(wsw,&WebSocketW::messageReceived,this,&Handler::onWswMessageReceived);
    //     connect(wsw,&WebSocketW::wswDisconnected,this,&Handler::initWsw);
    // }
    // qDebug() << "initWsw delay:" << delay;
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
}
void Handler::goPrevious(){
    vcbHandler->previous();
    emit updateListViewModel(vcbHandler->getModel());
}
