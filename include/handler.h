#ifndef CUSTOM_HANDLER_H
#define CUSTOM_HANDLER_H
    
    #include "database.h"
    #include <QObject>
    #include <QString>
    #include <QClipboard>
    #include <QStringListModel>
    #include "websocketw.h"
    #include "authenticate.h"
    #include "clip.h"
    #include "vcbhandler.h"
    //dbSchema : (textValue)

    class Handler: public QObject {
        Q_OBJECT

        public:

            explicit Handler(QObject* = nullptr);
            ~Handler();
            void addClipToSM(const Clip&);
            void addClipsToSM(const QList<Clip>*);
            void setSM(QStringListModel*const);
            void initWsw();
            void updatePrevClip(const Clip&);
            void syncOperation(const QJsonObject&);
            void goNext();
            void goPrevious();

        Q_SIGNALS:
            void makeCBSignal();
            void tokenExpired();
            void updateListViewModel(QStringListModel*);
        private Q_SLOTS:
            void onWssTokenExpired();
            void onVCBDataChanged(const Clip&,const QList<QString>&);       
            void onWswClipReceived(const Clip&,const QList<QString>&); 
            // void onModelUpdate(QStringListModel*);

        private:
            // Database *db;
            WebSocketW *wsw;
            // QClipboard* cb;
            VCBHandler *vcbHandler;
            QStringListModel* sm;
            QList<QString> server_head;
            QList<QString> device_head;
            Clip *prevClip;
            uint wsw_retries;
            Authenticate *authenticate;
    };

#endif