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
    using MenuTriggeredCallback = std::function<void()>;

private:
    void connectSignals();
    QMenu *createFloatButtonContextMenu(const QString &icon,
                                        const QString &text,
                                        MenuTriggeredCallback callback);
    void initFloatButtonEnabledContextMenu();
    void initFloatButtonDisabledContextMenu();
    void initTrayIcon(
        const FcitxVirtualKeyboardService &fcitxVirtualKeyboardService);

private:
    VirtualKeyboardManager &virtualKeyboardManager_;

    std::unique_ptr<FloatButtonManager> floatButtonManager_ = nullptr;

    std::unique_ptr<VirtualKeyboardTrayIcon> trayIconEntry_ = nullptr;

    std::shared_ptr<QMenu> floatButtonEnabledContextMenu_ = nullptr;
    std::shared_ptr<QMenu> floatButtonDisabledContextMenu_ = nullptr;
};

#endif // VIRTUALKEYBOARDENTRYMANAGER_H
