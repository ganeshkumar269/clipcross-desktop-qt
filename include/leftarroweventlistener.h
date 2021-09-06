#include <QObject>
class LeftArrowEventListener : public QObject
{
    Q_OBJECT
    protected:
        bool eventFilter(QObject *obj, QEvent *event) override;
    Q_SIGNALS:
        void clicked();
};
