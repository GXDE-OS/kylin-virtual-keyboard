#ifndef TRAYENTRY_H
#define TRAYENTRY_H

#include <QObject>
#include <QSystemTrayIcon>

#include "virtualkeyboard/virtualkeyboardmanager.h"
#include "virtualkeyboardentry/fcitxvirtualkeyboardservice.h"

class TrayEntry : public QObject {
    Q_OBJECT

public:
    TrayEntry(const FcitxVirtualKeyboardService &virtualKeyboardService,
              const VirtualKeyboardManager &virtualKeyboardManager);
    ~TrayEntry() = default;

private:
    void RegisterTrayEntry();

private slots:
    void ActiveTray(QSystemTrayIcon::ActivationReason reason);

private:
    QSystemTrayIcon *mSystemTray = nullptr;
    const VirtualKeyboardManager &virtualKeyboardManager_;
    const FcitxVirtualKeyboardService &virtualKeyboardService_;
};

#endif // TRAYENTRY_H
