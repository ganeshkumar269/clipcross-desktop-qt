#ifndef CUSTOM_CONSTANTS_H
#define CUSTOM_CONSTANTS_H
    #include <QString>
    #ifdef LOCAL_WSS_TEST
        const QString API_BASE_URL = "https://localhost:3000";     
        const QString API_WS_HOME_URL = "wss://localhost:3000";
        const QString WEB_SIGNIN_URL = "http://localhost:5000/api/signin";
    #else 
        const QString API_BASE_URL = "https://clippycross-server.ddns.net";     
        const QString API_WS_HOME_URL = "wss://clippycross-server.ddns.net";
        const QString WEB_SIGNIN_URL = "https://clipcross-web.vercel.app/api/signin";
    #endif
        const QString API_LOGIN_URL = API_BASE_URL + "/signin"; 
        const QString API_LOGOUT_URL = API_BASE_URL + "/logout"; 
        const QString API_TOKEN_URL = API_BASE_URL + "/refreshaccesstoken";
        const QString API_AUTH_URL = API_BASE_URL + "/auth";
#endif