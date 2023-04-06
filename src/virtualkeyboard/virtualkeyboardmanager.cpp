#include "virtualkeyboardmanager.h"

#include <QGuiApplication>
#include <QScreen>

#include "virtualkeyboardstrategy.h"

VirtualKeyboardManager::VirtualKeyboardManager(QObject *parent)
    : QObject(parent) {
    initVirtualKeyboardModel();

    initAppInputAreaManager();
    initPlacementModeManager();
    initGeometryManager();

    initScreenSignalConnections();
    initPlacementModeManagerSignalConnections();
}

VirtualKeyboardManager::~VirtualKeyboardManager() {
    hideVirtualKeyboard();

    placementModeManager_.reset();
    floatGeometryManager_.reset();
    expansionGeometryManager_.reset();
    appInputAreaManager_.reset();

    model_.reset();
}

void VirtualKeyboardManager::showVirtualKeyboard() {
    if (virtualkeyboardVisible_) {
        return;
    }
    initView();
    virtualkeyboardVisible_ = true;
    visibiltyChanged();
}

void VirtualKeyboardManager::hideVirtualKeyboard() {
    if (!virtualkeyboardVisible_) {
        return;
    }
    virtualkeyboardVisible_ = false;
    destoryView();
    visibiltyChanged();
}

void VirtualKeyboardManager::flipPlacementMode() {
    placementModeManager_->flipPlacementMode();
}

void VirtualKeyboardManager::moveBy(int offsetX, int offsetY) {
    floatGeometryManager_->moveBy(offsetX, offsetY);
}

void VirtualKeyboardManager::endDrag() { floatGeometryManager_->endDrag(); }

void VirtualKeyboardManager::visibiltyChanged() {
    emit virtualKeyboardVisibiltyChanged(virtualkeyboardVisible_);
    model_->processVisibilityEvent(virtualkeyboardVisible_);
}

bool VirtualKeyboardManager::isVirtualKeyboardVisible() const {
    return virtualkeyboardVisible_;
}

void VirtualKeyboardManager::updatePreeditCaret(int index) {
    emit model_->updatePreeditCaret(index);
}

void VirtualKeyboardManager::updatePreeditArea(const QString &preeditText) {
    emit model_->updatePreeditArea(preeditText);
}

void VirtualKeyboardManager::updateCandidateArea(
    const QStringList &candidateTextList, bool hasPrev, bool hasNext,
    int pageIndex, int globalCursorIndex) {
    model_->updateCandidateArea(QVariant(candidateTextList), hasPrev, hasNext,
                                pageIndex, globalCursorIndex);
}

void VirtualKeyboardManager::notifyIMActivated(const QString &uniqueName) {
    model_->setUniqueName(uniqueName);
}

void VirtualKeyboardManager::notifyIMDeactivated(
    const QString & /*uniqueName*/) {
    emit model_->imDeactivated();
}

void VirtualKeyboardManager::notifyIMListChanged() {
    model_->syncCurrentIMList();
}

void VirtualKeyboardManager::processResolutionChangedEvent() {
    if (virtualkeyboardVisible_) {
        placementModeManager_->updatePlacementMode();
    }
}

void VirtualKeyboardManager::initView() {
    view_.reset(new VirtualKeyboardView(this, model_.get()));

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

void VirtualKeyboardManager::initAppInputAreaManager() {
    appInputAreaManager_.reset(new AppInputAreaManager(this));
}

void VirtualKeyboardManager::initPlacementModeManager() {
    placementModeManager_.reset(new PlacementModeManager(viewSettings_));
}

void VirtualKeyboardManager::initGeometryManager() {
    floatGeometryManager_.reset(new FloatGeometryManager(
        std::unique_ptr<FloatGeometryManager::Strategy>(
            new VirtualKeyboardStrategy()),
        viewSettings_));
    expansionGeometryManager_.reset(new ExpansionGeometryManager());
}

void VirtualKeyboardManager::initVirtualKeyboardModel() {
    model_.reset(new VirtualKeyboardModel(this));
}

void VirtualKeyboardManager::connectVirtualKeyboardModelSignals() {
    connect(model_.get(), SIGNAL(updatePreeditArea(const QString &)),
            view_.get(), SIGNAL(updatePreeditArea(const QString &)));
    connect(model_.get(), SIGNAL(updateCandidateArea(const QVariant &, int)),
            view_.get(), SIGNAL(updateCandidateArea(const QVariant &, int)));
    connect(model_.get(), SIGNAL(imDeactivated()), view_.get(),
            SIGNAL(imDeactivated()));

    connect(model_.get(), SIGNAL(backendConnectionDisconnected()), this,
            SLOT(hideVirtualKeyboard()));
}

void VirtualKeyboardManager::connectGeometryManagerSignals() {
    connect(expansionGeometryManager_.get(), SIGNAL(viewMoved(int, int)),
            view_.get(), SLOT(move(int, int)));
    connect(expansionGeometryManager_.get(), SIGNAL(viewResized(int, int)),
            view_.get(), SLOT(resize(int, int)));
    connect(floatGeometryManager_.get(), SIGNAL(viewMoved(int, int)),
            view_.get(), SLOT(move(int, int)));
    connect(floatGeometryManager_.get(), SIGNAL(viewResized(int, int)),
            view_.get(), SLOT(resize(int, int)));
}

void VirtualKeyboardManager::connectPlacementModeManagerSignals() {
    connect(placementModeManager_.get(), SIGNAL(expansionModeEntered()),
            view_.get(), SIGNAL(expansionModeEntered()));

    connect(placementModeManager_.get(), SIGNAL(floatModeEntered()),
            view_.get(), SIGNAL(floatModeEntered()));
}

void VirtualKeyboardManager::connectSignals() {
    connectVirtualKeyboardModelSignals();

    connectPlacementModeManagerSignals();

    connectGeometryManagerSignals();
}

void VirtualKeyboardManager::raiseInputArea() {
    appInputAreaManager_->raiseInputArea(view_->geometry());
}

void VirtualKeyboardManager::fallInputArea() {
    appInputAreaManager_->fallInputArea();
}

void VirtualKeyboardManager::initScreenSignalConnections() {
    connect(QGuiApplication::primaryScreen(),
            SIGNAL(geometryChanged(const QRect &)), this,
            SLOT(processResolutionChangedEvent()));
}

void VirtualKeyboardManager::initPlacementModeManagerSignalConnections() {
    connect(placementModeManager_.get(), SIGNAL(expansionModeEntered()),
            expansionGeometryManager_.get(), SLOT(updateGeometry()));
    connect(placementModeManager_.get(), SIGNAL(expansionModeEntered()), this,
            SLOT(raiseInputArea()));

    connect(placementModeManager_.get(), SIGNAL(floatModeEntered()),
            floatGeometryManager_.get(), SLOT(updateGeometry()));
    connect(placementModeManager_.get(), SIGNAL(floatModeEntered()), this,
            SLOT(fallInputArea()));
}
