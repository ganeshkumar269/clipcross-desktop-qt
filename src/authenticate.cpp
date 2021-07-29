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
#include <QJsonObject>
#include "constants.h"

QT_USE_NAMESPACE

Authenticate::Authenticate(QObject*parent):QObject(parent){
    qDebug() << "Authenticate Constructor";
}

Authenticate::~Authenticate(){
    qDebug() << "Authenticate Destructor";
    if(server != nullptr)
        delete server;
}

void Authenticate::start(){
    if(hasTokensLocally()) {
        qDebug() << "Has Tokens Locally";
        QSettings s;
        qDebug() << "The available tokens are: ";
        qDebug() << "refresh_token : " << s.value("refresh_token");
        qDebug() << "id_token : " << s.value("id_token");
        qDebug() << "device_id : " << s.value("device_id");
        emit authenticated();
    }else{
        login();
    }
}

void Authenticate::login(){
    using namespace qhttp::server;
    if(server != nullptr) {
        stopAuthServer();
    }
    server = new qhttp::server::QHttpServer(this);
    server->listen(QHostAddress::Any, 12121,[&](QHttpRequest* req, QHttpResponse* res) {
            QUrl url(req->url());
            QString rt = getQueryParamValue(url,"refresh_token");
            QString did = getQueryParamValue(url,"device_id");
            QString idt = getQueryParamValue(url,"id_token");

            if(did != "" && rt != "" && idt != "" && did != "undefined" && rt != "undefined" && idt != "undefined" ){
                QSettings s;
                s.setValue("device_id",did);
                s.setValue("refresh_token",rt);
                s.setValue("id_token",idt);
                qDebug() << "Login SuccessFul";
                emit authenticated();
                res->setStatusCode(qhttp::ESTATUS_OK);
                res->write("<h2>Success, You Can close this tab!<h2>");
                res->end("Success!\n");
            }else{
                res->setStatusCode(qhttp::ESTATUS_OK);
                res->write("<h2>Unsuccessful, please try again!<h2>");
                res->end("Failure!\n");
            }
    });

    if(!server->isListening())  {
        qDebug() << "Server Not Listening";
        qDebug() << "Login Failed";
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
    QUrl url(API_TOKEN_URL);

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
    qDebug() << __FILE__ << __FUNCTION__ << " logout initiated";
    QUrl url(API_LOGOUT_URL);
    QNetworkAccessManager * mgr = new QNetworkAccessManager(this);
    QNetworkRequest req(url);
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    QJsonObject o;
    QSettings s;

    QString id_token = s.value("id_token").toString();
    QString email;
    auto tokenArray = id_token.split('.');
    if(tokenArray.length() == 3){
        QJsonDocument res = QJsonDocument::fromJson( QByteArray::fromBase64( tokenArray[1].toUtf8() ) );
        email = res.object()["email"].toString();
    }else{
        qDebug() << "Error Logging out, invalid id_token present";
    }

    qDebug() << "email : " << email; 

    o.insert("refresh_token",s.value("refresh_token").toString());
    o.insert("email",email);

    QByteArray data = QJsonDocument(o).toJson();
    QNetworkReply *r = mgr->post(req,data);

    #ifdef LOCAL_WSS_TEST
        QList<QSslCertificate> cert = QSslCertificate::fromPath(QLatin1String("./server.pem"));
        QSslError error1(QSslError::SelfSignedCertificate, cert.at(0));
        QSslError error2(QSslError::CertificateExpired, cert.at(0));
        QList<QSslError> expectedSslErrors;
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

void Authenticate::stopAuthServer(){
    server->stopListening();
}