#include "virtualkeyboardentry/virtualkeyboardtrayicon.h"

#include <QIcon>

VirtualKeyboardTrayIcon::VirtualKeyboardTrayIcon(
    VirtualKeyboardManager &virtualKeyboardManager,
    const FcitxVirtualKeyboardService &fcitxVirtualKeyboardService)
    : virtualKeyboardManager_(virtualKeyboardManager),
      fcitxVirtualKeyboardService_(fcitxVirtualKeyboardService) {
    initTrayIcon();
}

void VirtualKeyboardTrayIcon::setContextMenu(QMenu *contextMenu) {
    trayIcon_->setContextMenu(contextMenu);
}

void VirtualKeyboardTrayIcon::hideContextMenu() {
    trayIcon_->contextMenu()->hide();
}

void VirtualKeyboardTrayIcon::initTrayIcon() {
    trayIcon_ = new QSystemTrayIcon(this);
    trayIcon_->setIcon(QIcon::fromTheme("ukui-virtual-keyboard-symbolic"));
    trayIcon_->setToolTip(tr("kylin-virtual-keyboard"));
    connect(trayIcon_, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
            this, SLOT(onTrayIconActivated(QSystemTrayIcon::ActivationReason)));
    trayIcon_->setVisible(true);
}

void VirtualKeyboardTrayIcon::toggleVirtualKeyboard() {
    if (virtualKeyboardManager_.isVirtualKeyboardVisible()) {
        fcitxVirtualKeyboardService_.hideVirtualKeyboard();
    } else {
        fcitxVirtualKeyboardService_.showVirtualKeyboard();
    }
}

void VirtualKeyboardTrayIcon::onTrayIconActivated(
    QSystemTrayIcon::ActivationReason reason) {
    switch (reason) {
    case QSystemTrayIcon::Trigger: {
        toggleVirtualKeyboard();
        break;
    };
    default:
        break;
    }
}
