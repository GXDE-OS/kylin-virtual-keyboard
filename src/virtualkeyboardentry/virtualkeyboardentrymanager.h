#ifndef VIRTUALKEYBOARDENTRYMANAGER_H
#define VIRTUALKEYBOARDENTRYMANAGER_H

#include <functional>
#include <memory>

#include <QMenu>
#include <QObject>

#include "ipc/fcitxvirtualkeyboardserviceproxy.h"
#include "virtualkeyboard/virtualkeyboardmanager.h"
#include "virtualkeyboardentry/floatbuttonmanager.h"
#include "virtualkeyboardentry/virtualkeyboardtrayicon.h"

class VirtualKeyboardEntryManager : public QObject {
    Q_OBJECT

public:
    VirtualKeyboardEntryManager(
        VirtualKeyboardManager &virtualKeyboardManager,
        const FcitxVirtualKeyboardService &fcitxVirtualKeyboardService);
    ~VirtualKeyboardEntryManager() override;

private:
    using ActionTriggeredCallback = std::function<void()>;

private:
    void connectSignals();
    void initTrayIcon(
        const FcitxVirtualKeyboardService &fcitxVirtualKeyboardService);

    void initFloatButtonContextMenuAndAction();
    void updateFloatButtonContextMenuAction(const QString &icon,
                                            const QString &text,
                                            ActionTriggeredCallback callback);

private:
    VirtualKeyboardManager &virtualKeyboardManager_;

    std::unique_ptr<FloatButtonManager> floatButtonManager_ = nullptr;

    std::unique_ptr<VirtualKeyboardTrayIcon> trayIconEntry_ = nullptr;

    std::unique_ptr<QMenu> floatButtonContextMenu_ = nullptr;
    std::unique_ptr<QAction> floatButtonContextMenuAction_ = nullptr;
    ActionTriggeredCallback actionTriggeredCallback_;
};

#endif // VIRTUALKEYBOARDENTRYMANAGER_H
