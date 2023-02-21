#include "virtualkeyboardmanager.h"

#include <QDBusMetaType>
#include <QMetaType>

VirtualKeyboardManager::VirtualKeyboardManager(QObject *parent)
    : QObject(parent) {
    initDBusServiceWatcher();
    initDBusService();
    initAppInputAreaManager();
    initPlacementModeManager();
    initGeometryManager();
    initTrayEntry();

    initScreenSignalConnections();
}

VirtualKeyboardManager::~VirtualKeyboardManager() {
    HideVirtualKeyboard();
    serviceWatcher_.reset();
    virtualKeyboardBackendInterface_.reset();
    eventHandler_.reset();
    dBusService_.reset();
    placementModeManager_.reset();
    floatGeometryManager_.reset();
    expansionGeometryManager_.reset();
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
        const QString &imName = reply.value();
        emit changeIM(imName);
    }
    imChangedCall->deleteLater();
}

void VirtualKeyboardManager::hideVirtualKeyboard() { HideVirtualKeyboard(); }

class FcitxQtIMInfo {
public:
    friend QDBusArgument &operator<<(QDBusArgument &argument,
                                     const FcitxQtIMInfo &arg);
    friend const QDBusArgument &operator>>(const QDBusArgument &argument,
                                           FcitxQtIMInfo &arg);
    static void registerDBusMetaType();
    void setUniqueName(const QString &uniqueName) { uniqueName_ = uniqueName; }
    void setLocalName(const QString &localName) { localName_ = localName; }
    void setLabel(const QString &label) { label_ = label; }
    const QString &getUniqueName() const { return uniqueName_; }
    const QString &getLocalName() const { return localName_; }
    const QString &getLabel() const { return label_; }

private:
    QString uniqueName_;
    QString localName_;
    QString label_;
};

QDBusArgument &operator<<(QDBusArgument &argument, const FcitxQtIMInfo &arg) {
    argument.beginStructure();
    argument << arg.uniqueName_;
    argument << arg.localName_;
    argument << arg.label_;
    argument.endStructure();
    return argument;
}

const QDBusArgument &operator>>(const QDBusArgument &argument,
                                FcitxQtIMInfo &arg) {
    QString uniqueName;
    QString localName;
    QString label;
    argument.beginStructure();
    argument >> uniqueName >> localName >> label;
    argument.endStructure();
    arg.setUniqueName(uniqueName);
    arg.setLocalName(localName);
    arg.setLabel(label);
    return argument;
}

Q_DECLARE_METATYPE(FcitxQtIMInfo)

void FcitxQtIMInfo::registerDBusMetaType() {
    qDBusRegisterMetaType<FcitxQtIMInfo>();
    qDBusRegisterMetaType<QList<FcitxQtIMInfo>>();
}

void VirtualKeyboardManager::requestCurrentIMList() {
    FcitxQtIMInfo::registerDBusMetaType();
    QDBusPendingReply<QList<FcitxQtIMInfo>> reply =
        virtualKeyboardBackendInterface_->asyncCall("CurrentIMList");
    reply.waitForFinished();

    auto items = reply.value();

    QStringList stringList;
    for (const auto &imInfo : items) {
        stringList.append(imInfo.getUniqueName() + "|" + imInfo.getLocalName() +
                          "|" + imInfo.getLabel());
    }

    emit updateCurrentIMList(QVariant(stringList));
}

void VirtualKeyboardManager::processResolutionChangedEvent() {
    if (virtualkeyboardVisible_) {
        placementModeManager_->updatePlacementMode();
    }
}

void VirtualKeyboardManager::initView() {
    view_.reset(new VirtualKeyboardView(this));

    connectSignals();

    placementModeManager_->updatePlacementMode();
}

void VirtualKeyboardManager::destoryView() {
    appInputAreaManager_->fallInputArea();

    if (view_ == nullptr) {
        return;
    }

    view_.reset();
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

void VirtualKeyboardManager::initPlacementModeManager() {
    placementModeManager_.reset(new PlacementModeManager(this));
}

void VirtualKeyboardManager::initGeometryManager() {
    floatGeometryManager_.reset(new FloatGeometryManager(this));
    expansionGeometryManager_.reset(new ExpansionGeometryManager(this));
}

void VirtualKeyboardManager::connectVirtualKeyboardManagerSignals() {
    connect(this, SIGNAL(updatePreeditArea(const QString &)), view_.get(),
            SIGNAL(updatePreeditArea(const QString &)));
    connect(this,
            SIGNAL(updateCandidateArea(const QVariant &, bool, bool, int)),
            view_.get(),
            SIGNAL(updateCandidateArea(const QVariant &, bool, bool, int)));
    connect(this, SIGNAL(changeIM(const QString &)), view_.get(),
            SIGNAL(changeIM(const QString &)));
    connect(this, SIGNAL(reset()), view_.get(), SIGNAL(reset()));
    connect(this, SIGNAL(updateCurrentIMList(const QVariant &)), view_.get(),
            SIGNAL(updateCurrentIMList(const QVariant &)));
}

void VirtualKeyboardManager::connectGeometryManagerSignals() {
    connect(expansionGeometryManager_.get(),
            SIGNAL(virtualKeyboardMoved(int, int)), view_.get(),
            SLOT(moveVirtualKeyboard(int, int)));
    connect(expansionGeometryManager_.get(),
            SIGNAL(virtualKeyboardResized(int, int)), view_.get(),
            SLOT(resizeVirtualKeyboard(int, int)));
    connect(floatGeometryManager_.get(), SIGNAL(virtualKeyboardMoved(int, int)),
            view_.get(), SLOT(moveVirtualKeyboard(int, int)));
    connect(floatGeometryManager_.get(),
            SIGNAL(virtualKeyboardResized(int, int)), view_.get(),
            SLOT(resizeVirtualKeyboard(int, int)));
}

void VirtualKeyboardManager::connectRootObjectSignals() {
    const auto *rootObject = view_->rootObject();

    connect(rootObject, SIGNAL(qmlHideVirtualKeyboard()), this,
            SLOT(hideVirtualKeyboard()));
    connect(rootObject, SIGNAL(qmlRequestCurrentIMList()), this,
            SLOT(requestCurrentIMList()));

    connect(rootObject, SIGNAL(qmlPlacementModeButtonClicked()),
            placementModeManager_.get(), SLOT(flipPlacementMode()));

    connect(rootObject, SIGNAL(qmlMoveBy(int, int)),
            floatGeometryManager_.get(), SLOT(moveBy(int, int)));
    connect(rootObject, SIGNAL(qmlDragEnded()), floatGeometryManager_.get(),
            SLOT(endDrag()));

    eventHandler_->connectSignals(rootObject);
}

void VirtualKeyboardManager::initScreenSignalConnections() {
    connect(QGuiApplication::primaryScreen(),
            SIGNAL(geometryChanged(const QRect &)), this,
            SLOT(processResolutionChangedEvent()));
}

void VirtualKeyboardManager::connectPlacementModeManagerSignals() {
    connect(placementModeManager_.get(), SIGNAL(expansionModeEntered()),
            view_.get(), SIGNAL(expansionModeEntered()));
    connect(placementModeManager_.get(), SIGNAL(expansionModeEntered()),
            expansionGeometryManager_.get(), SLOT(updateGeometry()));
    connect(placementModeManager_.get(), SIGNAL(expansionModeEntered()), this,
            SLOT(raiseInputArea()));

    connect(placementModeManager_.get(), SIGNAL(floatModeEntered()),
            view_.get(), SIGNAL(floatModeEntered()));
    connect(placementModeManager_.get(), SIGNAL(floatModeEntered()),
            floatGeometryManager_.get(), SLOT(updateGeometry()));
    connect(placementModeManager_.get(), SIGNAL(floatModeEntered()), this,
            SLOT(fallInputArea()));
}

void VirtualKeyboardManager::connectSignals() {
    connectVirtualKeyboardManagerSignals();

    connectRootObjectSignals();

    connectPlacementModeManagerSignals();

    connectGeometryManagerSignals();
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

void VirtualKeyboardManager::raiseInputArea() {
    appInputAreaManager_->raiseInputArea(view_->geometry());
}

void VirtualKeyboardManager::fallInputArea() {
    appInputAreaManager_->fallInputArea();
}

void VirtualKeyboardManager::initTrayEntry() {
    trayEntry_.reset(new TrayEntry(this, this));
}
