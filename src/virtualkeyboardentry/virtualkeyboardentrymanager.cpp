#include "virtualkeyboardentry/virtualkeyboardentrymanager.h"

#include <QAction>
#include <QIcon>

#include "geometrymanager/geometrymanager.h"
#include "virtualkeyboardentry/floatbuttonstrategy.h"

VirtualKeyboardEntryManager::VirtualKeyboardEntryManager(
    VirtualKeyboardManager &virtualKeyboardManager,
    const FcitxVirtualKeyboardService &fcitxVirtualKeyboardService)
    : virtualKeyboardManager_(virtualKeyboardManager),
      floatButtonManager_(new FloatButtonManager(virtualKeyboardManager,
                                                 fcitxVirtualKeyboardService)) {
    initFloatButtonEnabledContextMenu();
    initFloatButtonDisabledContextMenu();
    initTrayIcon(fcitxVirtualKeyboardService);

    connectSignals();

    floatButtonManager_->loadFloatButtonAvailability();
}

VirtualKeyboardEntryManager::~VirtualKeyboardEntryManager() = default;

void VirtualKeyboardEntryManager::initTrayIcon(
    const FcitxVirtualKeyboardService &fcitxVirtualKeyboardService) {
    trayIconEntry_.reset(new VirtualKeyboardTrayIcon(
        virtualKeyboardManager_, fcitxVirtualKeyboardService));
}

void VirtualKeyboardEntryManager::connectSignals() {
    connect(floatButtonManager_.get(), &FloatButtonManager::floatButtonEnabled,
            this, [this]() {
                trayIconEntry_->setContextMenu(
                    floatButtonEnabledContextMenu_.get());
            });

    connect(floatButtonManager_.get(), &FloatButtonManager::floatButtonDisabled,
            this, [this]() {
                trayIconEntry_->setContextMenu(
                    floatButtonDisabledContextMenu_.get());
            });

    connect(&virtualKeyboardManager_,
            &VirtualKeyboardManager::virtualKeyboardVisibiltyChanged, this,
            [this](bool visible) {
                if (!visible) {
                    return;
                }

                trayIconEntry_->hideContextMenu();
            });
}

QMenu *VirtualKeyboardEntryManager::createFloatButtonContextMenu(
    const QString &icon, const QString &text, MenuTriggeredCallback callback) {
    QMenu *menu = new QMenu();

    QAction *action = new QAction(menu);
    action->setIcon(QIcon(icon));
    action->setText(text);

    menu->addAction(action);

    connect(action, &QAction::triggered, this, [callback](bool) {
        if (!callback) {
            return;
        }

        callback();
    });

    return menu;
}

void VirtualKeyboardEntryManager::initContextMenuSignalConnection(QMenu *menu) {
    connect(menu, &QMenu::aboutToShow, this,
            [this]() { virtualKeyboardManager_.hide(); });
}

void VirtualKeyboardEntryManager::initFloatButtonEnabledContextMenu() {
    floatButtonEnabledContextMenu_.reset(createFloatButtonContextMenu(
        ":/floatbutton/img/disablefloatbutton.svg",
        tr("Disable the float button"),
        [this]() { floatButtonManager_->disableFloatButton(); }));

    initContextMenuSignalConnection(floatButtonEnabledContextMenu_.get());
}

void VirtualKeyboardEntryManager::initFloatButtonDisabledContextMenu() {
    floatButtonDisabledContextMenu_.reset(createFloatButtonContextMenu(
        ":/floatbutton/img/enablefloatbutton.svg",
        tr("Enable the float button"),
        [this]() { floatButtonManager_->enableFloatButton(); }));

    initContextMenuSignalConnection(floatButtonDisabledContextMenu_.get());
}
