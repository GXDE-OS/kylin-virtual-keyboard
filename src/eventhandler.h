#ifndef EVENTHANDLER_H
#define EVENTHANDLER_H

#include <QObject>
#include <QQuickItem>

class VirtualKeyboardManager;

class EventHandler : public QObject {
    Q_OBJECT

public:
    explicit EventHandler(VirtualKeyboardManager *virtualKeyboardManager,
                          QObject *parent = nullptr);
    ~EventHandler() = default;
    void connectSignals(const QObject *rootObject);

public slots:
    void SelectCandidate(int index);
    void SetCurrentInputMethod(const QString &imName);
    void ProcessKeyEvent(const QString &keyval, int keycode, int state,
                         bool isRelease, int time);

private:
    VirtualKeyboardManager *virtualKeyboardManager_;
};

#endif // EVENTHANDLER_H
