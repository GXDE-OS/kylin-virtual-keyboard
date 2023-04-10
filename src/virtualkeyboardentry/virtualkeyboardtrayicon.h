#ifndef VIRTUALKEYBOARDTRAYICON_H
#define VIRTUALKEYBOARDTRAYICON_H

#include <memory>

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

    void setContextMenu(std::shared_ptr<QMenu> contextMenu);
    void hideContextMenu();

private:
    void initTrayIcon();

    void toggleVirtualKeyboard();

    void ensuareVirtualKeyboardInvisible();
    void showContextMenu();

private slots:
    void onTrayIconActivated(QSystemTrayIcon::ActivationReason reason);

private:
    QSystemTrayIcon *trayIcon_ = nullptr;
    std::shared_ptr<QMenu> contextMenu_ = nullptr;

    VirtualKeyboardManager &virtualKeyboardManager_;
    const FcitxVirtualKeyboardService &fcitxVirtualKeyboardService_;
};

#endif // VIRTUALKEYBOARDTRAYICON_H
