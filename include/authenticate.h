#ifndef custom_authenticate_h
    #define custom_authenticate_h
    #include <QObject>
    #include <QJsonObject>
    #include "qhttp/qhttpserver.hpp"
    class Authenticate : public QObject{
        Q_OBJECT
        public:
            explicit Authenticate(QObject* =nullptr);
            ~Authenticate();
            void start();
            bool hasTokensLocally();
            void refreshAccessToken();
            void logout();
            void login();
            void stopAuthServer();
        Q_SIGNALS:
            void authenticated();
            void accessTokenRefreshed();
            void loggedout();
        private:
            qhttp::server::QHttpServer *server = nullptr;
    };  
#endif