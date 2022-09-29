#include "virtualkeyboardmanager.h"

VirtualKeyboardManager::VirtualKeyboardManager(QObject *parent)
    : QObject(parent) {
    initDBusServiceWatcher();
    initDBusService();
    initAppInputAreaManager();
}

VirtualKeyboardManager::~VirtualKeyboardManager() {
    HideVirtualKeyboard();
    serviceWatcher_.reset();
    virtualKeyboardBackendInterface_.reset();
    eventHandler_.reset();
    dBusService_.reset();
    appInputAreaManager_.reset();
}

void VirtualKeyboardManager::ShowVirtualKeyboard() {
    if (virtualkeyboardVisible_) {
        return;
    }
    initView();
    virtualkeyboardVisible_ = true;
    VisibiltyChanged();
}

void VirtualKeyboardManager::HideVirtualKeyboard() {
    if (!virtualkeyboardVisible_) {
        return;
    }
    virtualkeyboardVisible_ = false;
    destoryView();
    VisibiltyChanged();
}

void VirtualKeyboardManager::VisibiltyChanged() {
    virtualKeyboardBackendInterface_->asyncCall("ProcessVisibilityEvent",
                                                virtualkeyboardVisible_);
}

bool VirtualKeyboardManager::IsVirtualKeyboardVisible() {
    return virtualkeyboardVisible_;
}

void VirtualKeyboardManager::UpdatePreeditCaret(int index) {
    emit updatePreeditCaret(index);
}

void VirtualKeyboardManager::UpdatePreeditArea(const QString &preeditText) {
    emit updatePreeditArea(preeditText);
}

void VirtualKeyboardManager::UpdateCandidateArea(
    const QStringList &candidateTextList, bool hasPrev, bool hasNext,
    int pageIndex) {
    emit updateCandidateArea(QVariant(candidateTextList), hasPrev, hasNext,
                             pageIndex);
}

void VirtualKeyboardManager::NotifyIMActivated(const QString &uniqueName) {
    emit changeIM(uniqueName);
}

void VirtualKeyboardManager::NotifyIMDeactivated(
    const QString & /*uniqueName*/) {
    emit reset();
}

void VirtualKeyboardManager::NotifyIMListChanged() {
    fcitx::FcitxQtControllerProxy fcitxQtControllerProxy(
        fcitx5Service, fcitx5ServiceControllerPath,
        QDBusConnection::sessionBus(), this);
    QDBusPendingReply<QString> reply =
        fcitxQtControllerProxy.CurrentInputMethod();
    QDBusPendingCallWatcher *watcher = new QDBusPendingCallWatcher(reply, this);
    QObject::connect(watcher, SIGNAL(finished(QDBusPendingCallWatcher *)), this,
                     SLOT(imListChanged(QDBusPendingCallWatcher *)));
}

void VirtualKeyboardManager::imListChanged(
    QDBusPendingCallWatcher *imChangedCall) {
    QDBusPendingReply<QString> reply = *imChangedCall;
    if (!reply.isError()) {
        const QString& imName = reply.value();
        emit changeIM(imName);
    }
    imChangedCall->deleteLater();
}

void VirtualKeyboardManager::hideVirtualKeyboard() { HideVirtualKeyboard(); }

void VirtualKeyboardManager::requestCurrentIMList() {
    QDBusPendingReply<QString> reply =
        fcitx5ControllerInterface_->asyncCall("CurrentInputMethodGroup");
    QDBusPendingCallWatcher *watcher = new QDBusPendingCallWatcher(reply, this);
    QObject::connect(watcher, &QDBusPendingCallWatcher::finished,
                     [this](QDBusPendingCallWatcher *watcher) {
                         QDBusPendingReply<QString> reply = *watcher;
                         watcher->deleteLater();

                         if (reply.isError()) {
                             return;
                         }

                         updateIMList(reply.value());
                     });
}

void VirtualKeyboardManager::updateIMList(const QString &currentIMGroup) {
    fcitx::FcitxQtControllerProxy fcitxQtControllerProxy(
        fcitx5Service, fcitx5ServiceControllerPath,
        QDBusConnection::sessionBus(), this);
    fcitx::FcitxQtStringKeyValueList items;
    auto reply =
        fcitxQtControllerProxy.InputMethodGroupInfo(currentIMGroup, items);
    if (!reply.isValid()) {
        return;
    }

    currentIMList_.clear();
    for (const auto &imInfo : items) {
        currentIMList_.append(imInfo.key());
    }

    emit updateCurrentIMList(QVariant(currentIMList_));
}

void VirtualKeyboardManager::resizeView() {
    QScreen *screen = QGuiApplication::primaryScreen();
    QRect deskRect = screen->geometry();
    int width = deskRect.width();
    int height = deskRect.width() * virtualKeyboardAspectRatio_;
    int x = 0;
    int y = deskRect.height() - height;
    view_->setGeometry(x, y, width, height);
}

void VirtualKeyboardManager::processResolutionChangedEvent() {
    if (virtualkeyboardVisible_) {
        resizeView();
        appInputAreaManager_->raiseInputArea(view_.get(), view_->geometry());
    }
}

void VirtualKeyboardManager::initView() {
    view_.reset(new QQuickView());
    view_->setSource(QUrl("qrc:/qml/VirtualKeyboard.qml"));
    view_->setFlag(Qt::WindowStaysOnTopHint);
    KWindowSystem::setType(view_->winId(), NET::Dock);

    connectSignals();

    resizeView();
    appInputAreaManager_->raiseInputArea(view_.get(), view_->geometry());

    view_->show();
}

void VirtualKeyboardManager::destoryView() {
    if (view_ == nullptr) {
        return;
    }
    if (view_->isVisible()) {
        view_->hide();
    }
    view_.release()->deleteLater();
}

void VirtualKeyboardManager::initDBusServiceWatcher() {
    serviceWatcher_.reset(new QDBusServiceWatcher(this));
    serviceWatcher_->setConnection(QDBusConnection::sessionBus());
    serviceWatcher_->addWatchedService(virtualKeyboardBackendService);
    serviceWatcher_->setWatchMode(QDBusServiceWatcher::WatchForRegistration |
                                  QDBusServiceWatcher::WatchForUnregistration);
    connect(serviceWatcher_.get(), SIGNAL(serviceRegistered(const QString &)),
            this, SLOT(backendServiceRegistered(const QString &)));
    connect(serviceWatcher_.get(), SIGNAL(serviceUnregistered(const QString &)),
            this, SLOT(backendServiceUnregistered(const QString &)));
}

void VirtualKeyboardManager::initVirtualKeyboardBackendInterface() {
    virtualKeyboardBackendInterface_.reset(new QDBusInterface(
        virtualKeyboardBackendService, virtualKeyboardBackendServicePath,
        virtualKeyboardBackendServiceInterface, QDBusConnection::sessionBus(),
        this));
}

void VirtualKeyboardManager::initFcitx5ControllerInterface() {
    fcitx5ControllerInterface_.reset(new QDBusInterface(
        fcitx5Service, fcitx5ServiceControllerPath,
        fcitx5ServiceControllerInterface, QDBusConnection::sessionBus(), this));
}

void VirtualKeyboardManager::initEventHandler() {
    eventHandler_.reset(new EventHandler(this, this));
}

void VirtualKeyboardManager::initAppInputAreaManager() {
    appInputAreaManager_.reset(new AppInputAreaManager(this));
}

void VirtualKeyboardManager::connectSignals() {
    const auto *rootObject = view_->rootObject();
    connect(this, SIGNAL(updatePreeditArea(const QString &)), rootObject,
            SIGNAL(qmlUpdatePreedit(QString)));
    connect(this,
            SIGNAL(updateCandidateArea(const QVariant &, bool, bool, int)),
            rootObject, SIGNAL(qmlUpdateCandidateList(QVariant)));
    connect(this, SIGNAL(changeIM(const QString &)), rootObject,
            SIGNAL(qmlChangeIm(QString)));
    connect(this, SIGNAL(reset()), rootObject, SIGNAL(qmlReset()));
    connect(this, SIGNAL(updateCurrentIMList(const QVariant &)), rootObject,
            SIGNAL(qmlUpdateCurrentIMList(QVariant)));

    connect(QGuiApplication::primaryScreen(),
            SIGNAL(geometryChanged(const QRect)), this,
            SLOT(processResolutionChangedEvent()));

    connect(rootObject, SIGNAL(qmlHideVirtualKeyboard()), this,
            SLOT(hideVirtualKeyboard()));
    connect(rootObject, SIGNAL(qmlRequestCurrentIMList()), this,
            SLOT(requestCurrentIMList()));

    eventHandler_->connectSignals(rootObject);
}

void VirtualKeyboardManager::initDBusService() {
    dBusService_.reset(new DBusService(this, this));
}

void VirtualKeyboardManager::backendServiceRegistered(
    const QString &serviceName) {
    if (serviceName != virtualKeyboardBackendService) {
        return;
    }
    initVirtualKeyboardBackendInterface();
    initFcitx5ControllerInterface();
    initEventHandler();
}

void VirtualKeyboardManager::backendServiceUnregistered(
    const QString &serviceName) {
    if (serviceName != virtualKeyboardBackendService) {
        return;
    }
    HideVirtualKeyboard();
    virtualKeyboardBackendInterface_.reset();
    eventHandler_.reset();
}
