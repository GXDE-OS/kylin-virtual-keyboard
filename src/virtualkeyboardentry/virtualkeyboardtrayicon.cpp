#include <QIcon>

#include "virtualkeyboard/virtualkeyboardmanager.h"
#include "virtualkeyboardentry/virtualkeyboardtrayicon.h"

VirtualKeyboardTrayIcon::VirtualKeyboardTrayIcon(
    VirtualKeyboardManager &virtualKeyboardManager,
    const FcitxVirtualKeyboardService &fcitxVirtualKeyboardService)
    : virtualKeyboardManager_(virtualKeyboardManager),
      fcitxVirtualKeyboardService_(fcitxVirtualKeyboardService) {
    initTrayIcon();
}

void VirtualKeyboardTrayIcon::initTrayIcon() {
    trayIcon_ = new QSystemTrayIcon(this);
    trayIcon_->setIcon(QIcon::fromTheme("input-keyboard-symbolic"));
    trayIcon_->setToolTip(tr("kylin-virtual-keyboard"));
    connect(trayIcon_, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
            this, SLOT(onTrayIconActivated(QSystemTrayIcon::ActivationReason)));
    trayIcon_->setVisible(true);
}

void VirtualKeyboardTrayIcon::onTrayIconActivated(
    QSystemTrayIcon::ActivationReason reason) {
    switch (reason) {
    case QSystemTrayIcon::Trigger: {
        virtualKeyboardManager_.isVirtualKeyboardVisible()
            ? fcitxVirtualKeyboardService_.hideVirtualKeyboard()
            : fcitxVirtualKeyboardService_.showVirtualKeyboard();
        break;
    };
    default:
        break;
    }
}
