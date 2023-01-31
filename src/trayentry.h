#ifndef TRAYENTRY_H
#define TRAYENTRY_H
#include <QObject>
#include <QSystemTrayIcon>
#include <QDBusInterface>

class VirtualKeyboardManager;

class TrayEntry : public QObject {
    Q_OBJECT

public:
    explicit TrayEntry(VirtualKeyboardManager *virtualKeyboardManager,
                       QObject *parent = nullptr);
    ~TrayEntry() = default;

private:
    void RegisterTrayEntry();
    void ConnectFcitxVirtualKeyboardBackend();
    void CallDbusMethodShowVirtualKeyboard();
    void CallDbusMethodHideVirtualKeyboard();

private slots:
    void ActiveTray(QSystemTrayIcon::ActivationReason reason);

private:
    QSystemTrayIcon* mSystemTray = nullptr;
    VirtualKeyboardManager *virtualKeyboardManager_ = nullptr;
    QDBusInterface *virtualKeyboardBackendInterface_ = nullptr;
    QString dbusName_ = "org.fcitx.virtualkeyboard.service";
    QString dbusPath_ = "/virtualkeyboard";
    QString dbusInterface_ = "org.fcitx.virtualkeyboard.service";
};

#endif // TRAYENTRY_H