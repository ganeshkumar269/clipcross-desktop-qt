#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H
    #include <QWidget>
    class MainWindow : public QWidget {
        Q_OBJECT
        public : 
            explicit MainWindow(QWidget* parent = nullptr);
            bool nativeEvent(const QByteArray &eventType, void *message, qintptr *result);
        Q_SIGNALS:
            void registeredShortcutTriggered(MSG*);
    };
#endif