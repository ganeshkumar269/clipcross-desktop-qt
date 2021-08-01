
#ifndef custom_vcbw_h
    #define custom_vcbw_h
    #include <QObject>
    #include "clip.h"
    #include "vcb.h"
    #include <QPair>

    class VCBHandler : public QObject{
        Q_OBJECT
        public:
            explicit VCBHandler(QObject* =nullptr);
            ~VCBHandler();
            void add(const Clip&);
            void add(const Clip&,const QString&, const QString& deviceId = "desktop"); //clip,id
            void add(const Clip&,const QStringList&); //clip,ids

            Clip getTopClip();
            Clip getTopClip(const QString&);//id
            QStringListModel* getModel();
            QStringListModel* getModel(const QString&);
            Clip getClipAtIndex(int);
            // void updateTopClip(const Clip&);
            QString getVisibleVCBId();
            QList<QString> getActiveVCBIds();
            void next();
            void previous();
            QList<QPair<QString,Clip>> vcbTopClips();
            bool hasVcbId(const QString& id);
            
        Q_SIGNALS:
            void vcbDataChanged(const Clip&,const QList<QString> /*vcbIds*/);
        public Q_SLOTS:   
            void onCbDataChanged();
        private:
            QHash<QString,VCB*> vcbList;
            QList<QString> activeVCBIds;
            QString visibleVCBId;
            int visibleVCBIdIndex;
            QStringList* vcbListOrder;
            // Clip *topClip;
            // QList<Clip> getTopClips;
            QClipboard* cb;
    };  
#endif