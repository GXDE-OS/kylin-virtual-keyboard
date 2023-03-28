#include "trayentry.h"

#include <QIcon>

#include "virtualkeyboard/virtualkeyboardmanager.h"

TrayEntry::TrayEntry(const FcitxVirtualKeyboardService &virtualKeyboardService,
                     VirtualKeyboardManager *virtualKeyboardManager)
    : virtualKeyboardManager_(virtualKeyboardManager),
      virtualKeyboardService_(virtualKeyboardService) {
    RegisterTrayEntry();
}

void TrayEntry::RegisterTrayEntry() {
    mSystemTray = new QSystemTrayIcon(this);
    mSystemTray->setIcon(QIcon::fromTheme("input-keyboard-symbolic"));
    mSystemTray->setToolTip(tr("kylin-virtual-keyboard"));
    connect(mSystemTray, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
            this, SLOT(ActiveTray(QSystemTrayIcon::ActivationReason)));
    mSystemTray->setVisible(true);
}

void TrayEntry::ActiveTray(QSystemTrayIcon::ActivationReason reason) {
    switch (reason) {
    case QSystemTrayIcon::Trigger: {
        virtualKeyboardManager_->IsVirtualKeyboardVisible()
            ? virtualKeyboardService_.hideVirtualKeyboard()
            : virtualKeyboardService_.showVirtualKeyboard();
        break;
    };
    default:
        break;
    }
}
