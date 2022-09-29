#include "dbusservice.h"
#include "virtualkeyboardmanager.h"

DBusService::DBusService(VirtualKeyboardManager *virtualKeyboardManager,
                         QObject *parent)
    : QObject(parent), virtualKeyboardManager_(virtualKeyboardManager) {
    startService();
}

DBusService::~DBusService() { stopService(); }

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
    virtualKeyboardManager_->ShowVirtualKeyboard();
}

void DBusService::HideVirtualKeyboard() {
    virtualKeyboardManager_->HideVirtualKeyboard();
}

bool DBusService::IsVirtualKeyboardVisible() {
    return virtualKeyboardManager_->IsVirtualKeyboardVisible();
}

void DBusService::UpdatePreeditCaret(int preeditCursor) {
    virtualKeyboardManager_->UpdatePreeditCaret(preeditCursor);
}

void DBusService::UpdatePreeditArea(const QString &preeditText) {
    virtualKeyboardManager_->updatePreeditArea(preeditText);
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
