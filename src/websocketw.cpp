#include "websocketw.h"
#include "utils.h"

#include <QtWebSockets/QWebSocket>
#include <QtNetwork/QSslError>
#include <QUrl>
#include <QAbstractSocket>
#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>

/*
    Handler -> wsw
    WebSokcetW -> wss
        
*/

WebSocketW::WebSocketW(QObject* parent):QObject(parent){
    qDebug() << "[websocketw.cpp] WebSocketW Constructor";
    createWss();
}
WebSocketW::~WebSocketW(){
    qDebug() << "[websokcetw.cpp] Destructor";
    delete wss;
}
void WebSocketW::createWss(){
    if(wss != nullptr) delete wss;
    wss = new QWebSocket();
    connect(wss,&QWebSocket::connected,this,&WebSocketW::onWssConnected);
    connect(wss,&QWebSocket::disconnected,this,&WebSocketW::onWssDisconnected);
    connect(wss,QOverload<const QList<QSslError>&>::of(&QWebSocket::sslErrors),this,&WebSocketW::onSslErrors);
    connect(wss,
            QOverload<QAbstractSocket::SocketError>::of(&QWebSocket::error),
            this,
            &WebSocketW::onWssError
            );
    connect(wss,
            QOverload<QAbstractSocket::SocketState>::of(&QWebSocket::stateChanged),
            this,
            &WebSocketW::onWssStateChanged
            );
    #ifdef LOCAL_WSS_TEST
        QList<QSslCertificate> cert = QSslCertificate::fromPath(QLatin1String("./server.pem"));
        QSslError error1(QSslError::SelfSignedCertificate, cert.at(0));
        QSslError error2(QSslError::CertificateExpired, cert.at(0));
        QList<QSslError> expectedSslErrors;
        expectedSslErrors.append(error1);
        expectedSslErrors.append(error2);
        wss->ignoreSslErrors(expectedSslErrors);
        wss->open(getWssUrl());
    #else
        wss->open(getWssUrl()); 
    #endif
}

void WebSocketW::createWssWithDelay(uint delay){
    QTimer::singleShot(delay,this,[&](){
        createWss();
    });
}

void WebSocketW::wssAuthenticate(){
    QVariant idt = settings->value("id_token");
    QVariant did = settings->value("device_id");
    if(!idt.isNull()){
        QJsonObject o;
        o.insert("id_token",idt.toString());
        o.insert("device_id",did.toString());
        o.insert("authentication",true);
        wss->sendTextMessage(QJsonDocument(o).toJson(QJsonDocument::Compact));
    }
}

void WebSocketW::onWssConnected()
{
    qDebug() << "WebSocket connected";
    connect(wss, &QWebSocket::textMessageReceived,
            this, &WebSocketW::onWssMessageReceived);
    initSyncFlow();
}

void WebSocketW::onWssDisconnected()
{
    qDebug() << "WebSocket Disconnected";
    auto closeCode = wss->closeCode();
    // closeCode.
    qDebug() << __FILE__ << __FUNCTION__ << closeCode;
    qDebug() << __FILE__ << __FUNCTION__ << wss->closeReason();

    if(closeCode == 4003) {//id_token expired check the spec for custom close code
        QJsonObject o = QJsonDocument(QJsonDocument::fromJson(wss->closeReason().toUtf8())).object();
        if(o.contains("expired_token") && o["expired_token"].toBool() == true)
            emit wssTokenExpired();
    }
    

    // emit wswDisconnected(wss->closeCode(),wss->closeReason());
}

void WebSocketW::onWssStateChanged(QAbstractSocket::SocketState state){
    qDebug() << "State Changed" << state;
}
void WebSocketW::onSslErrors(const QList<QSslError>& errors)
{ 
    qDebug() << "SSL  Errors";
    for(const QSslError& i : errors){
        qDebug() << "SslError: " << i ; 
    }
}

void WebSocketW::onWssError(QAbstractSocket::SocketError error){
    qDebug() << "[websocketw.cpp] error: " << error;
    qDebug() << "[websocketw.cpp] socketstate" << wss->state();
    switch(error){
        case QAbstractSocket::SocketError::RemoteHostClosedError : 
                {
                    wss->disconnect();
                    qDebug() << "RemoteHostClosedError";
                    createWssWithDelay();
                }
                break;
        case QAbstractSocket::SocketError::ConnectionRefusedError: 
                {
                    qDebug() << "Connection";
                    if(wss->state() == QAbstractSocket::SocketState::UnconnectedState){
                        qDebug() << "No connection made in the fist place";
                        createWssWithDelay();
                    }
                    if(wss->state() == QAbstractSocket::SocketState::ConnectingState){
                        qDebug() << "Connecting State Error ignored";
                    }
                }
                break;
        default:
                {
                    qDebug() << "ErrorString " << wss->errorString();
                }
    }
}

void WebSocketW::onWssMessageReceived(const QString& message)
{
    qDebug() << "WebSocket Message Received: " << message;
    // rapidjson::Document d;
    // d.Parse(message.toStdString().c_str());
    // if(d.HasParseError()){
    //     qDebug() << "[websocketw.cpp] ParseError:" << d.GetParseError();
    //     return;
    // }
    // if(!(d.HasMember("value") && d.HasMember("format"))){
    //     qDebug() << "[websocketw.cpp] Missing members in received json";
    //     return;
    // }
    // auto value  = d["value"].GetString();
    // auto format = d["format"].GetString();
    QJsonDocument d = QJsonDocument::fromJson(message.toUtf8());
    QJsonObject o = d.object();
    if(o.contains("authenticate")){
        if(o["authenticate"].toBool() == true)
            if(o.contains("status") && o["status"].toString() == "200")
                emit wssAuthenticated();
    }
    else if(o.contains("expired_token") && o["expired_token"].toBool() == true){
        emit wssTokenExpired();
    }   
    else if(o.contains("syncflow") && o["syncflow"].toBool() == true){

        // Clip clip(o["value"].toString(),o["format"].toString(),o["timestamp"].toInteger());
        o.remove("syncflow");
        emit syncFlowDataReceived(o);
    }
    else if(o.contains("isClip") && o["isClip"].toBool() == true){

        // o.contains("value") && o.contains("format") && o.contains("timestamp") && o.contains("vcbIds")){
        
        QJsonObject clipObj = o["clip"].toObject();
        
        Clip clip(
                    clipObj["value"].toString(),
                    clipObj["format"].toString(),
                    clipObj["timestamp"].toInteger()
                );
        QJsonArray id_array = o["vcbIds"].toArray();
        QStringList t;
        for(QVariant i : id_array.toVariantList())
            t.append(i.toString());
        emit clipReceived(clip,t);
    }
    else {
        qDebug() << __FILE__ << __FUNCTION__ << "Invalid JSON Message";
    }
}


void WebSocketW::sendClip(const Clip& clip,const QList<QString>& ids)
{
    qDebug() << "[websocketw.cpp] sendWssData "; //<< value_string << " " << format_string;
    if(!wss->isValid()) return;
    // QString machine_id = QSysInfo::machineUniqueId();
    // QJsonObject o;
    // o.insert("value",value_string);
    // o.insert("format",format_string);
    // o.insert("id_token",settings->value("id_token").toString());
    QJsonDocument doc = QJsonDocument::fromJson(clip.toJson().toStdString().c_str());
    QJsonObject message;
    auto obj = doc.object();
    message.insert("clip",obj);
    QJsonArray json_id_array;
    for(QString id : ids)
        json_id_array.append(id);
    message.insert("vcbIds",json_id_array);
    wss->sendTextMessage(QJsonDocument(message).toJson());
}

void WebSocketW::initSyncFlow(){
    QJsonObject o;
    o.insert("syncflow",true);
    wss->sendTextMessage(QJsonDocument(o).toJson(QJsonDocument::Compact));
}

void WebSocketW::onPong(quint64 elapsedTime, const QByteArray &payload){
    qDebug() << "websokcetw.cpp onPong payload: " << payload;
}

// void WebSocketW::syncWithServer(){
    
// }
