#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H
    #include <QWidget>
    #include <QKeySequence>
    class MainWindow : public QWidget {
        Q_OBJECT
        public : 
            explicit MainWindow(QWidget* parent = nullptr);
            bool nativeEvent(const QByteArray &eventType, void *message, qintptr *result);
            // void changeEvent(QEvent*);
        // protected:
        //     void closeEvent(QCloseEvent *event) override;

        Q_SIGNALS:
            void registeredShortcutTriggered(MSG*);
            void registeredHotkeyActivated(const QKeySequence&);
        // private:
        //     bool closing;
    };
#endif