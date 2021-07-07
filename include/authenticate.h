#ifndef custom_authenticate_h
    #define custom_authenticate_h
    #include <QObject>
    #include <QJsonObject>
    #include "qhttpserver.hpp"
    class Authenticate : public QObject{
        Q_OBJECT
        public:
            explicit Authenticate(QObject* =nullptr);
            ~Authenticate();
            void start();
            bool hasTokensLocally();
            void refreshAccessToken();
        Q_SIGNALS:
            void authenticated();
            void accessTokenRefreshed();
        private:
            qhttp::server::QHttpServer *server;
    };  
#endif