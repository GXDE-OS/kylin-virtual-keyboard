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
    initTrayIcon(fcitxVirtualKeyboardService);

    initFloatButtonContextMenuAndAction();

    connectSignals();

    floatButtonManager_->loadFloatButtonAvailability();
}

VirtualKeyboardEntryManager::~VirtualKeyboardEntryManager() = default;

void VirtualKeyboardEntryManager::initTrayIcon(
    const FcitxVirtualKeyboardService &fcitxVirtualKeyboardService) {
    trayIconEntry_.reset(new VirtualKeyboardTrayIcon(
        virtualKeyboardManager_, fcitxVirtualKeyboardService));
}

void VirtualKeyboardEntryManager::initFloatButtonContextMenuAndAction() {
    floatButtonContextMenu_.reset(new QMenu);
    floatButtonContextMenuAction_.reset(new QAction);

    floatButtonContextMenu_->addAction(floatButtonContextMenuAction_.get());

    trayIconEntry_->setContextMenu(floatButtonContextMenu_.get());

    connect(floatButtonContextMenu_.get(), &QMenu::aboutToShow, this,
            [this]() { virtualKeyboardManager_.hide(); });

    connect(floatButtonContextMenuAction_.get(), &QAction::triggered, this,
            [this](bool) {
                if (!actionTriggeredCallback_) {
                    return;
                }

                actionTriggeredCallback_();
            });
}

void VirtualKeyboardEntryManager::connectSignals() {
    connect(floatButtonManager_.get(), &FloatButtonManager::floatButtonEnabled,
            this, [this]() {
                updateFloatButtonContextMenuAction(
                    ":/floatbutton/img/disablefloatbutton.svg",
                    tr("Disable the float button"),
                    [this]() { floatButtonManager_->disableFloatButton(); });
            });

    connect(floatButtonManager_.get(), &FloatButtonManager::floatButtonDisabled,
            this, [this]() {
                updateFloatButtonContextMenuAction(
                    ":/floatbutton/img/enablefloatbutton.svg",
                    tr("Enable the float button"),
                    [this]() { floatButtonManager_->enableFloatButton(); });
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

void VirtualKeyboardEntryManager::updateFloatButtonContextMenuAction(
    const QString &icon, const QString &text,
    ActionTriggeredCallback callback) {
    floatButtonContextMenuAction_->setIcon(QIcon(icon));
    floatButtonContextMenuAction_->setText(text);

    actionTriggeredCallback_ = std::move(callback);
}
