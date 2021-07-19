#include "authenticate.h"
#include "qhttp/qhttpserver.hpp"
#include "qhttp/qhttpserverconnection.hpp"
#include "qhttp/qhttpserverrequest.hpp"
#include "qhttp/qhttpserverresponse.hpp"
#include <QDesktopServices>
#include "utils.h"
#include <QJsonDocument>
#include <QSettings>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QObject>
QT_USE_NAMESPACE
// #include "log4qt/logger.h"
// auto logger = Log4Qt::Logger::rootLogger(); 

Authenticate::Authenticate(QObject*parent):QObject(parent){
    qDebug() << __FILE__ << __FUNCTION__ << __LINE__;
}

Authenticate::~Authenticate(){
    qDebug() << "Authenticate.cpp Authenticate Destructor";
    if(server != nullptr)
        delete server;
}

void Authenticate::start(){
    if(hasTokensLocally()) {
        qDebug() << "Has Tokens Locally";
        emit authenticated();
    }else{
        login();
    }
}

void Authenticate::login(){
    using namespace qhttp::server;
    server = new qhttp::server::QHttpServer(this);
    server->listen(QHostAddress::Any, 12121,[&](QHttpRequest* req, QHttpResponse* res) {
            qDebug() << __FILE__ << __FUNCTION__ << "request handler";

            QUrl url(req->url());
            QString rt = getQueryParamValue(url,"refresh_token");
            QString did = getQueryParamValue(url,"device_id");
            QString idt = getQueryParamValue(url,"id_token");

            if(did != "" && rt != "" && idt != ""){
                QSettings s;
                s.setValue("device_id",did);
                s.setValue("refresh_token",rt);
                s.setValue("id_token",idt);
                qDebug() << __FILE__ << __FUNCTION__ << " Login SuccessFul";
                emit authenticated();
            }
            res->setStatusCode(qhttp::ESTATUS_OK);
            res->write("<h2>Success, You Can close this tab!<h2>");
            res->end("Success!\n");
    });

    if(!server->isListening())  {
        qDebug() << "Server Not Listening";
        qDebug() << __FILE__ << __FUNCTION__ << " Login Failed";
    }
    else{
        qDebug() << "Server Listening";
        QDesktopServices::openUrl(getOAuthUrl());
    }
}

bool Authenticate::hasTokensLocally(){
    QSettings s;
    return  s.contains("refresh_token") && 
            s.contains("id_token") &&
            s.contains("device_id");
}

void Authenticate::refreshAccessToken(){
    QUrl url;
    #ifdef LOCAL_WSS_TEST
        url = QUrl("https://localhost:3000/refreshaccesstoken");
    #else
        url = QUrl("https://wss-node-server.ganeshkumar269.repl.co/refreshaccesstoken");
    #endif

    QNetworkAccessManager * mgr = new QNetworkAccessManager(this);
    QNetworkRequest req(url);
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    QJsonObject o;
    QSettings s;
    o.insert("refresh_token",s.value("refresh_token").toString());
    QByteArray data = QJsonDocument(o).toJson();

    QList<QSslCertificate> cert = QSslCertificate::fromPath(QLatin1String("./server.pem"));
    QSslError error1(QSslError::SelfSignedCertificate, cert.at(0));
    QSslError error2(QSslError::CertificateExpired, cert.at(0));
    QList<QSslError> expectedSslErrors;
    QNetworkReply *r = mgr->post(req,data);
    #ifdef LOCAL_WSS_TEST
        expectedSslErrors.append(error1);
        expectedSslErrors.append(error2);
        r->ignoreSslErrors(expectedSslErrors);
    #endif
    

    QObject::connect(r, &QNetworkReply::finished,this,[=](){
        if(r->error() == QNetworkReply::NoError){
            QString contents = QString::fromUtf8(r->readAll());
            qDebug() << __FILE__ << __FUNCTION__ << contents;
            QJsonObject o = QJsonDocument(QJsonDocument::fromJson(contents.toStdString().c_str())).object();
            
            if(o.contains("id_token")){
                QSettings t;
                t.setValue("id_token",o["id_token"].toString());
                emit accessTokenRefreshed();
            }else{
                qDebug() << __FILE__ << __FUNCTION__ << "No id_token found in the response";
            }
        }
        else{
            QString err = r->errorString();
            qDebug() << "[authenticate.cpp] refreshAccessToken " << err;
        }
        r->deleteLater();
    });
}

void Authenticate::logout(){
    QUrl url;
    qDebug() << __FILE__ << __FUNCTION__ << " logout initiated";
    #ifdef LOCAL_WSS_TEST
        url = QUrl("https://localhost:3000/logout");
    #else
        url = QUrl("https://wss-node-server.ganeshkumar269.repl.co/logout");
    #endif

    QNetworkAccessManager * mgr = new QNetworkAccessManager(this);
    QNetworkRequest req(url);
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    QJsonObject o;
    QSettings s;
    o.insert("refresh_token",s.value("refresh_token").toString());
    QByteArray data = QJsonDocument(o).toJson();

    QList<QSslCertificate> cert = QSslCertificate::fromPath(QLatin1String("./server.pem"));
    QSslError error1(QSslError::SelfSignedCertificate, cert.at(0));
    QSslError error2(QSslError::CertificateExpired, cert.at(0));
    QList<QSslError> expectedSslErrors;
    QNetworkReply *r = mgr->post(req,data);
    #ifdef LOCAL_WSS_TEST
        expectedSslErrors.append(error1);
        expectedSslErrors.append(error2);
        r->ignoreSslErrors(expectedSslErrors);
    #endif
    

    QObject::connect(r, &QNetworkReply::finished,this,[=](){
        if(r->error() == QNetworkReply::NoError){
            QString contents = QString::fromUtf8(r->readAll());
            qDebug() << __FILE__ << __FUNCTION__ << contents;
            QJsonObject o = QJsonDocument(QJsonDocument::fromJson(contents.toStdString().c_str())).object();
            
            if(o.contains("status") && o["status"].toInt() == 200){
                qDebug() << "Logout Successful";
                emit loggedout();
            }else{
                qDebug() << __FILE__ << __FUNCTION__ << "Logout Unsuccessful";
            }
        }
        else{
            QString err = r->errorString();
            qDebug() << "[authenticate.cpp] logout " << err;
        }
        r->deleteLater();
    });
    
}