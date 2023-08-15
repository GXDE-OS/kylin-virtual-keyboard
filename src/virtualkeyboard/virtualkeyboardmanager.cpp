/*
 * Copyright 2022 KylinSoft Co., Ltd.
 *
 * This program is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later
 * version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program. If not, see <https://www.gnu.org/licenses/>.
 */

#include "virtualkeyboardmanager.h"

#include <QGuiApplication>
#include <QScreen>

#include "virtualkeyboardstrategy.h"

VirtualKeyboardManager::VirtualKeyboardManager(
    HideVirtualKeyboardCallback hideVirtualKeyboardCallback)
    : hideVirtualKeyboardCallback_(std::move(hideVirtualKeyboardCallback)) {
    initVirtualKeyboardModel();

    initAppInputAreaManager();
    initPlacementModeManager();
    initGeometryManager();

    initVirtualKeyboardView();

    initScreenSignalConnections();
    initPlacementModeManagerSignalConnections();
}

VirtualKeyboardManager::~VirtualKeyboardManager() {
    hideVirtualKeyboard();

    placementModeManager_.reset();
    floatGeometryManager_.reset();
    expansionGeometryManager_.reset();
    appInputAreaManager_.reset();

    view_.reset();
    model_.reset();
}

void VirtualKeyboardManager::showVirtualKeyboard() {
    if (isVirtualKeyboardVisible()) {
        return;
    }

    view_->show();

    if (!placementModeManager_->isFloatMode()) {
        appInputAreaManager_->raiseInputArea(view_->geometry());
    }

    visibiltyChanged();
}

void VirtualKeyboardManager::hideVirtualKeyboard() {
    if (!isVirtualKeyboardVisible()) {
        return;
    }

    appInputAreaManager_->fallInputArea();

    view_->hide();

    visibiltyChanged();
}

void VirtualKeyboardManager::hide() {
    if (!hideVirtualKeyboardCallback_) {
        return;
    }

    hideVirtualKeyboardCallback_();
}

void VirtualKeyboardManager::flipPlacementMode() {
    placementModeManager_->flipPlacementMode();
}

void VirtualKeyboardManager::moveBy(int offsetX, int offsetY) {
    floatGeometryManager_->moveBy(offsetX, offsetY);
}

void VirtualKeyboardManager::endDrag() { floatGeometryManager_->endDrag(); }

void VirtualKeyboardManager::visibiltyChanged() {
    emit virtualKeyboardVisibiltyChanged(isVirtualKeyboardVisible());
}

bool VirtualKeyboardManager::isVirtualKeyboardVisible() const {
    return view_->isVisible();
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
    if (isVirtualKeyboardVisible()) {
        placementModeManager_->updatePlacementMode();
    }
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

    connect(model_.get(), SIGNAL(backendConnectionDisconnected()), this,
            SLOT(hideVirtualKeyboard()));
}

std::shared_ptr<GeometryManager>
VirtualKeyboardManager::getCurrentGeometryManager() const {
    if (placementModeManager_->isFloatMode()) {
        return floatGeometryManager_;
    }

    return expansionGeometryManager_;
}

void VirtualKeyboardManager::initVirtualKeyboardView() {
    view_.reset(
        new VirtualKeyboardView(*this, *model_, getCurrentGeometryManager()));

    connectVirtualKeyboardModelSignals();

    connectGeometryManagerSignals();
}

void VirtualKeyboardManager::connectVirtualKeyboardModelSignals() {
    connect(model_.get(), SIGNAL(updatePreeditArea(const QString &)),
            view_.get(), SIGNAL(updatePreeditArea(const QString &)));
    connect(model_.get(), SIGNAL(updateCandidateArea(const QVariant &, int)),
            view_.get(), SIGNAL(updateCandidateArea(const QVariant &, int)));
    connect(model_.get(), SIGNAL(imDeactivated()), view_.get(),
            SIGNAL(imDeactivated()));
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

void VirtualKeyboardManager::initScreenSignalConnections() {
    connect(QGuiApplication::primaryScreen(),
            SIGNAL(geometryChanged(const QRect &)), this,
            SLOT(processResolutionChangedEvent()));
}

void VirtualKeyboardManager::onExpansionModeEntered() {
    view_->setGeometryManager(expansionGeometryManager_);
    expansionGeometryManager_->updateGeometry();
    appInputAreaManager_->raiseInputArea(view_->geometry());

    emit isFloatModeChanged();
}

void VirtualKeyboardManager::onFloatModeEntered() {
    view_->setGeometryManager(floatGeometryManager_);
    floatGeometryManager_->updateGeometry();
    appInputAreaManager_->fallInputArea();

    emit isFloatModeChanged();
}

void VirtualKeyboardManager::initPlacementModeManagerSignalConnections() {
    connect(placementModeManager_.get(),
            &PlacementModeManager::expansionModeEntered, this,
            &VirtualKeyboardManager::onExpansionModeEntered);

    connect(placementModeManager_.get(),
            &PlacementModeManager::floatModeEntered, this,
            &VirtualKeyboardManager::onFloatModeEntered);
}
