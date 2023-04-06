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
        [this]() { virtualKeyboardManager_->showVirtualKeyboard(); },
        [this]() { virtualKeyboardManager_->hideVirtualKeyboard(); },
        [this]() {
            return !virtualKeyboardManager_->isVirtualKeyboardVisible();
        },
        [this]() {
            return virtualKeyboardManager_->isVirtualKeyboardVisible();
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
    return virtualKeyboardManager_->isVirtualKeyboardVisible();
}

void DBusService::UpdatePreeditCaret(int preeditCursor) {
    virtualKeyboardManager_->updatePreeditCaret(preeditCursor);
}

void DBusService::UpdatePreeditArea(const QString &preeditText) {
    virtualKeyboardManager_->updatePreeditArea(preeditText);
}

void DBusService::UpdateCandidateArea(const QStringList &candidateTextList,
                                      bool hasPrev, bool hasNext, int pageIndex,
                                      int globalCursorIndex /* = -1*/) {
    virtualKeyboardManager_->updateCandidateArea(
        candidateTextList, hasPrev, hasNext, pageIndex, globalCursorIndex);
}

void DBusService::NotifyIMActivated(const QString &uniqueName) {
    virtualKeyboardManager_->notifyIMActivated(uniqueName);
}

void DBusService::NotifyIMDeactivated(const QString &uniqueName) {
    virtualKeyboardManager_->notifyIMDeactivated(uniqueName);
}

void DBusService::NotifyIMListChanged() {
    virtualKeyboardManager_->notifyIMListChanged();
}
