#include "trayentry.h"
#include "virtualkeyboardmanager.h"
#include <QIcon>
#include <QDBusMessage>

TrayEntry::TrayEntry(VirtualKeyboardManager *virtualKeyboardManager)
:  virtualKeyboardManager_(virtualKeyboardManager) {
    ConnectFcitxVirtualKeyboardBackend();
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

void TrayEntry::ConnectFcitxVirtualKeyboardBackend() {
    virtualKeyboardBackendInterface_ = new
            QDBusInterface(dbusName_,dbusPath_,dbusInterface_,QDBusConnection::sessionBus());
    if (!virtualKeyboardBackendInterface_->isValid()) {
        return;
    }
}

void TrayEntry::CallDbusMethodShowVirtualKeyboard() {
    QDBusMessage replyMsg =
            virtualKeyboardBackendInterface_->call("ShowVirtualKeyboard");
    if (replyMsg.type() == QDBusMessage::MessageType::ErrorMessage) {
        return;
    }
}

void TrayEntry::CallDbusMethodHideVirtualKeyboard() {
    QDBusMessage replyMsg =
            virtualKeyboardBackendInterface_->call("HideVirtualKeyboard");
    if (replyMsg.type() == QDBusMessage::MessageType::ErrorMessage) {
        return;
    }
}

void TrayEntry::ActiveTray(QSystemTrayIcon::ActivationReason reason) {
    switch(reason) {
        case QSystemTrayIcon::Trigger: {
        virtualKeyboardManager_->IsVirtualKeyboardVisible() ?
                    CallDbusMethodHideVirtualKeyboard() : CallDbusMethodShowVirtualKeyboard();
        break;
    };
        default:
            break;
    }
}
