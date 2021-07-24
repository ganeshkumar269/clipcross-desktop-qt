#ifndef CUSTOM_WEBSOCKETW_H
#define CUSTOM_WEBSOCKETW_H
    #include <QObject>
    #include <QtWebSockets/QWebSocket>
    #include <QTimer>
    #include <QSettings>
    #include "clip.h"
    class WebSocketW:public QObject{
        Q_OBJECT
        public:
            explicit WebSocketW(QObject* = nullptr);
            ~WebSocketW();
            void sendClip(const Clip&,const QList<QString>&);
            void createWssWithDelay(uint =1500); 
            void createWss();
            void wssAuthenticate();
            void initSyncFlow(QList<QPair<QString,Clip>>);
        private:
            void pingUntilReadyToTransferData();
        Q_SIGNALS:
            void clipReceived(const Clip&,const QList<QString>&);
            void wswDisconnected(QWebSocketProtocol::CloseCode,const QString&);
            void wssAuthenticated();
            void wssTokenExpired();
            void wssConnectionEstablished();
            void wssReadyToTransferData();
            void syncFlowDataReceived(const QJsonObject&);
        private Q_SLOTS:
            void onWssConnected();
            void onWssDisconnected();
            void onSslErrors(const QList<QSslError>&);
            void onWssMessageReceived(const QString&);
            void onWssStateChanged(QAbstractSocket::SocketState);
            void onWssError(QAbstractSocket::SocketError);
            void onPong(quint64, const QByteArray&);
        private:
            QWebSocket *wss=nullptr;
            QSettings *settings=nullptr;
            bool canTransferData = false;
            QTimer *timer;
    };
#endif
