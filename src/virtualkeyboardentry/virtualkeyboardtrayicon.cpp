#include "virtualkeyboardentry/virtualkeyboardtrayicon.h"

#include <QApplication>
#include <QDesktopWidget>
#include <QIcon>

VirtualKeyboardTrayIcon::VirtualKeyboardTrayIcon(
    VirtualKeyboardManager &virtualKeyboardManager,
    const FcitxVirtualKeyboardService &fcitxVirtualKeyboardService)
    : virtualKeyboardManager_(virtualKeyboardManager),
      fcitxVirtualKeyboardService_(fcitxVirtualKeyboardService) {
    initTrayIcon();
}

void VirtualKeyboardTrayIcon::setContextMenu(
    std::shared_ptr<QMenu> contextMenu) {
    contextMenu_ = contextMenu;
}

void VirtualKeyboardTrayIcon::hideContextMenu() { contextMenu_->hide(); }

void VirtualKeyboardTrayIcon::initTrayIcon() {
    trayIcon_ = new QSystemTrayIcon(this);
    trayIcon_->setIcon(QIcon::fromTheme("input-keyboard-symbolic"));
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

void VirtualKeyboardTrayIcon::ensuareVirtualKeyboardInvisible() {
    if (!virtualKeyboardManager_.isVirtualKeyboardVisible()) {
        return;
    }

    virtualKeyboardManager_.hideVirtualKeyboard();
}

void VirtualKeyboardTrayIcon::showContextMenu() {
    QSize menuSize = contextMenu_->sizeHint();
    QPoint point = QCursor::pos();
    QRect deskRect = QApplication::desktop()->availableGeometry();
    contextMenu_->move(point.x(), deskRect.height() - menuSize.height());
    contextMenu_->show();
}

void VirtualKeyboardTrayIcon::onTrayIconActivated(
    QSystemTrayIcon::ActivationReason reason) {
    switch (reason) {
    case QSystemTrayIcon::Trigger: {
        toggleVirtualKeyboard();
        break;
    };
    case QSystemTrayIcon::Context: {
        ensuareVirtualKeyboardInvisible();
        showContextMenu();
        break;
    };
    default:
        break;
    }
}
