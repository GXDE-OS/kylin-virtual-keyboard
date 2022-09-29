#ifndef DBUSSERVICE_H
#define DBUSSERVICE_H

#include <QDBusConnection>
#include <QObject>

class VirtualKeyboardManager;

class DBusService : public QObject {
    Q_OBJECT

public:
    explicit DBusService(VirtualKeyboardManager *virtualKeyboardManager,
                         QObject *object = nullptr);
    ~DBusService();

private:
    bool startService();
    bool stopService();

public slots:
    void ShowVirtualKeyboard();
    void HideVirtualKeyboard();
    bool IsVirtualKeyboardVisible();
    void UpdatePreeditCaret(int preeditCursor);
    void UpdatePreeditArea(const QString &preeditText);
    void UpdateCandidateArea(const QStringList &candidateTextList, bool hasPrev,
                             bool hasNext, int pageIndex);
    void NotifyIMActivated(const QString &uniqueName);
    void NotifyIMDeactivated(const QString &uniqueName);
    void NotifyIMListChanged();

private:
    VirtualKeyboardManager *virtualKeyboardManager_ = nullptr;
    QString serviceName_ = "org.fcitx.Fcitx5.VirtualKeyboard";
    QString servicePath_ = "/org/fcitx/virtualkeyboard/impanel";
    QString serviceInterface_ = "org.fcitx.Fcitx5.VirtualKeyboard1";
};

#endif // DBUSSERVICE_H
