#include "dbusservice.h"
#include "virtualkeyboard/virtualkeyboardmanager.h"

DBusService::DBusService(VirtualKeyboardManager *virtualKeyboardManager)
    : virtualKeyboardManager_(virtualKeyboardManager),
      virtualKeyboardVisibilityRequestMerger_(
          VIRTUAL_KEYBOARD_VISIBILITY_PEROID) {
    initRequestMerger();

    startService();
}

DBusService::~DBusService() { stopService(); }

void DBusService::initRequestMerger() {
    virtualKeyboardVisibilityRequestMerger_.init(
        [this]() { virtualKeyboardManager_->ShowVirtualKeyboard(); },
        [this]() { virtualKeyboardManager_->HideVirtualKeyboard(); },
        [this]() {
            return !virtualKeyboardManager_->IsVirtualKeyboardVisible();
        },
        [this]() {
            return virtualKeyboardManager_->IsVirtualKeyboardVisible();
        });
}

bool DBusService::startService() {
    return QDBusConnection::sessionBus().registerService(serviceName_) &&
           QDBusConnection::sessionBus().registerObject(
               servicePath_, serviceInterface_, this,
               QDBusConnection::ExportAllSlots);
}

bool DBusService::stopService() {
    QDBusConnection::sessionBus().unregisterObject(servicePath_);
    return QDBusConnection::sessionBus().unregisterService(serviceName_);
}

void DBusService::ShowVirtualKeyboard() {
    virtualKeyboardVisibilityRequestMerger_.activate();
}

void DBusService::HideVirtualKeyboard() {
    virtualKeyboardVisibilityRequestMerger_.deactivate();
}

bool DBusService::IsVirtualKeyboardVisible() {
    return virtualKeyboardManager_->IsVirtualKeyboardVisible();
}

void DBusService::UpdatePreeditCaret(int preeditCursor) {
    virtualKeyboardManager_->UpdatePreeditCaret(preeditCursor);
}

void DBusService::UpdatePreeditArea(const QString &preeditText) {
    virtualKeyboardManager_->UpdatePreeditArea(preeditText);
}

void DBusService::UpdateCandidateArea(const QStringList &candidateTextList,
                                      bool hasPrev, bool hasNext,
                                      int pageIndex) {
    virtualKeyboardManager_->UpdateCandidateArea(candidateTextList, hasPrev,
                                                 hasNext, pageIndex);
}

void DBusService::NotifyIMActivated(const QString &uniqueName) {
    virtualKeyboardManager_->NotifyIMActivated(uniqueName);
}

void DBusService::NotifyIMDeactivated(const QString &uniqueName) {
    virtualKeyboardManager_->NotifyIMDeactivated(uniqueName);
}

void DBusService::NotifyIMListChanged() {
    virtualKeyboardManager_->NotifyIMListChanged();
}
