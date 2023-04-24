#ifndef VIRTUALKEYBOARDTRAYICON_H
#define VIRTUALKEYBOARDTRAYICON_H

#include <QMenu>
#include <QObject>
#include <QSystemTrayIcon>

#include "virtualkeyboard/virtualkeyboardmanager.h"
#include "virtualkeyboardentry/fcitxvirtualkeyboardservice.h"

class VirtualKeyboardTrayIcon : public QObject {
    Q_OBJECT

public:
    VirtualKeyboardTrayIcon(
        VirtualKeyboardManager &virtualKeyboardManager,
        const FcitxVirtualKeyboardService &fcitxVirtualKeyboardService);

    ~VirtualKeyboardTrayIcon() override = default;

    void setContextMenu(QMenu *contextMenu);
    void hideContextMenu();

private:
    void initTrayIcon();

    void toggleVirtualKeyboard();

private slots:
    void onTrayIconActivated(QSystemTrayIcon::ActivationReason reason);

private:
    QSystemTrayIcon *trayIcon_ = nullptr;

    VirtualKeyboardManager &virtualKeyboardManager_;
    const FcitxVirtualKeyboardService &fcitxVirtualKeyboardService_;
};

#endif // VIRTUALKEYBOARDTRAYICON_H
