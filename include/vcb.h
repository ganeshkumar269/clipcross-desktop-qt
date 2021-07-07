#ifndef custom_vcb_h
    #define custom_vcb_h
    #include <QObject>
    #include <QSqlDatabase>
    #include "clip.h"
    #include "database.h"
    class VCB : public QObject{
        Q_OBJECT
        public:
            explicit VCB(const QString& id,QObject* =nullptr);
            ~VCB();
            void add(const Clip&);
            void add(const QModelIndex&);
            void addClipToSlm(const Clip&);
            void addClipsToSlm(const QList<Clip>*);
            Clip getTopClip();
            QStringListModel* getModel();
        Q_SIGNALS:

        public Q_SLOTS:   
            // void onListViewDoubleClicked(const QModelIndex&);
        private:
            const QString id; //permanent
            QString name; //visible to user and customizable
            QStringListModel* slm; //create a custom listmodel
            QList<Clip>* clips;
            Clip topClip;
            Clip lastSyncedClip;
            Database *db;
    };  
#endif