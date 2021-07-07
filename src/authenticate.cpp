#include "authenticate.h"
#include "qhttpserver.hpp"
#include "qhttpserverconnection.hpp"
#include "qhttpserverrequest.hpp"
#include "qhttpserverresponse.hpp"
#include <QDesktopServices>
#include "utils.h"
#include <QJsonDocument>
#include <QSettings>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QObject>
QT_USE_NAMESPACE

Authenticate::Authenticate(QObject*parent):QObject(parent){
    qDebug() << __FILE__ << __FUNCTION__ << __LINE__;
    start();
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
        return;
    }
    using namespace qhttp::server;
    server = new qhttp::server::QHttpServer(this);
    server->listen(QHostAddress::Any, 12121,[&](QHttpRequest* req, QHttpResponse* res) {
            qDebug() << __FILE__ << __FUNCTION__ << "request made";

            // qDebug() << req->url();
            QUrl url(req->url());
            // qDebug() << "url.query " << url.query();
            // qDebug() << "url.toStringList" << url.toStringList();
            QString at = getQueryParamValue(url,"access_token"); //secure it later with qtkeychain
            QString rt = getQueryParamValue(url,"refresh_token");
            QString idt = getQueryParamValue(url,"id_token");
            if(at != "" && rt != "" && idt != ""){
                QSettings s;
                s.setValue("access_token",at);
                s.setValue("refresh_token",rt);
                s.setValue("id_token",idt);
                s.setValue("device_id",getMd5Hash(rt));

                emit authenticated();
            }
            // http status 200
            res->setStatusCode(qhttp::ESTATUS_OK);
            // the response body data
            // res->end("<h2>Success, You Can close this tab!<h2>");
            res->write("<h2>Success, You Can close this tab!<h2>");
            res->end("Success!\n");
            // automatic memory management for req/res
    });
    if(!server->isListening())  qDebug() << "Server Not Listening";
    else{
        qDebug() << "Server Listening";
        QDesktopServices::openUrl(getOAuthUrl());
    }
}

bool Authenticate::hasTokensLocally(){
    QSettings s;
    // verifyExpiry();
    return  s.contains("access_token") && 
            s.contains("refresh_token") && 
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
            QSettings t;
            t.setValue("access_token",o["access_token"].toString());
            t.setValue("id_token",o["id_token"].toString());
            emit accessTokenRefreshed();
        }
        else{
            QString err = r->errorString();
            qDebug() << "[authenticate.cpp] refreshAccessToken " << err;
        }
        r->deleteLater();
    });

}