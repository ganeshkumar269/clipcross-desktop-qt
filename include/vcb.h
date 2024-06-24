#ifndef custom_vcb_h
    #define custom_vcb_h
    #include <QObject>
    #include <QSqlDatabase>
    #include "clip.h"
    #include "database.h"
    class VCB : public QObject{
        Q_OBJECT
        public:
            explicit VCB(const QString& id,const QString& deviceId, QObject* =nullptr);
            ~VCB();
            void add(const Clip&);
            void removeFromSlm(int);
            void add(const QModelIndex&);
            void checkForDuplicateAndAdd(const Clip& clip);
            void addClipToSlm(const Clip&);
            void addClipsToSlm(const QList<Clip>*);
            Clip getTopClip();
            Clip getClipAtIndex(int);
            QStringListModel* getModel();
            QList<Clip>* onSearchQuery(const QString&);
        Q_SIGNALS:

        public Q_SLOTS:   
            // void onListViewDoubleClicked(const QModelIndex&);
        private:
            const QString id; //permanent
            QString name; //visible to user and customizable
            QStringListModel* slm = nullptr; //create a custom listmodel
            QStringListModel* searchResult = nullptr;
            QList<Clip>* clips = nullptr;
            Clip topClip;
            Clip lastSyncedClip;
            Database *db = nullptr;
            const QString deviceId;
    };
#endif