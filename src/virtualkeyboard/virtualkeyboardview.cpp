/*
 * Copyright (c) KylinSoft Co., Ltd. 2022.All rights reserved.
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

#include "virtualkeyboard/virtualkeyboardview.h"
#include <QQmlContext>
#include <QQuickItem>
#include "screenmanager.h"
#include "themewatcher.h"
#include "ukuiwaylandhelper/ukuiwaylandproperties.h"
#include "utils.h"
#include "virtualkeyboardsettings/virtualkeyboardsettings.h"

VirtualKeyboardView::VirtualKeyboardView(
    QObject &manager, QObject &model,
    std::unique_ptr<PlacementModeManager> placementModeManager,
    std::unique_ptr<ExpansionGeometryManager> expansionGeometryManager,
    std::unique_ptr<FloatGeometryManager> floatGeometryManager,
    ThemeWatcher &themeWatcher)
    : manager_(manager), model_(model), themeWatcher_(themeWatcher),
      placementModeManager_(std::move(placementModeManager)),
      expansionGeometryManager_(std::move(expansionGeometryManager)),
      floatGeometryManager_(std::move(floatGeometryManager)) {

    connect(placementModeManager_.get(),
            &PlacementModeManager::isFloatModeChanged, this,
            &VirtualKeyboardView::isFloatModeChanged);
    connect(floatGeometryManager_.get(), &FloatGeometryManager::viewMoved, this,
            &VirtualKeyboardView::move);
    connect(floatGeometryManager_.get(), &FloatGeometryManager::viewResized,
            this, &VirtualKeyboardView::resize);
    connect(
        &VirtualKeyboardSettings::getInstance(),
        &VirtualKeyboardSettings::animationAvailabilityChanged, this, [this]() {
            if (!VirtualKeyboardSettings::getInstance().isAnimationEnabled() &&
                view_) {
                view_->setOpacity(1.0f);
            }
        });
    initState();
    if (VirtualKeyboardSettings::getInstance().isPreloadViewEnabled()) {
        KVKBD_INFO("will preload quickview");
        initView();
    }
}

VirtualKeyboardView::~VirtualKeyboardView() {
    destroyView();
    if (view_ != nullptr) {
        view_.release()->deleteLater();
    }

    currentState_.reset();
    visibleState_.reset();
    hidingState_.reset();
    showingState_.reset();
    invisibleState_.reset();
    flippingState_.reset();
}

void VirtualKeyboardView::moveBy(int offsetX, int offsetY) {
    if (!isFloatMode()) {
        return;
    }

    floatGeometryManager_->moveBy(offsetX, offsetY);
}

void VirtualKeyboardView::endDrag() {
    if (!isFloatMode()) {
        return;
    }

    floatGeometryManager_->endDrag();
}

QRect VirtualKeyboardView::geometry() const {
    return getCurrentGeometryManager().geometry();
}

QRect VirtualKeyboardView::screenGeometry() const {
    return getCurrentGeometryManager().screenGeometry();
}

void VirtualKeyboardView::updateGeometry() {
    if (!isVisible()) {
        return;
    }

    view_->setGeometry(geometry());

    emitContentGeometrySignals();
}

void VirtualKeyboardView::updateExpansionFlippingStartGeometry() {
    auto geometry = floatGeometryManager_->geometry();
    view_->setGeometry(geometry.x(), view_->y(), geometry.width(),
                       geometry.height());

    emitContentGeometrySignals();
}

void VirtualKeyboardView::move(int x, int y) {
    if (!view_) {
        KVKBD_WARN("view_ is null!");
        return;
    }
    view_->setX(x);
    view_->setY(y);
}

void VirtualKeyboardView::resize() { updateGeometry(); }

void VirtualKeyboardView::initView() {
    const auto preloadViewEnabled =
        VirtualKeyboardSettings::getInstance().isPreloadViewEnabled();
    if (view_ != nullptr && preloadViewEnabled) {
        return;
    }
    KVKBD_INFO("preloadViewEnabled:{}", preloadViewEnabled);

    view_.reset(new QQuickView());

    view_->rootContext()->setContextProperty("manager", &manager_);
    view_->rootContext()->setContextProperty("model", &model_);
    view_->rootContext()->setContextProperty("view", this);
    view_->rootContext()->setContextProperty("themeWatcher", &themeWatcher_);

    view_->rootContext()->setContextProperty("QT_VERSION_MAJOR",
                                             QT_VERSION_MAJOR);
    view_->rootContext()->setContextProperty("QT_VERSION_MINOR",
                                             QT_VERSION_MINOR);
    view_->rootContext()->setContextProperty("QT_VERSION_PATCH",
                                             QT_VERSION_PATCH);

    view_->setTitle("kylin-virtual-keyboard");
    view_->setColor(QColor(Qt::transparent));
    view_->setSource(QUrl("qrc:/qml/VirtualKeyboard.qml"));

    if (getDesktopEnvironment() == DesktopEnvironment::UKUI &&
        getDesktopType() == DesktopType::WAYLAND) {
        view_->setProperty(UkuiWaylandProperty::SURFACE_ROLE,
                           UkuiWaylandProperty::Role::INPUT_PANEL);
        view_->setProperty(UkuiWaylandProperty::SURFACE_NO_TITLEBAR, true);
        QPair<QRegion, int> pair(QRegion(), 0);
        view_->setProperty(UkuiWaylandProperty::SURFACE_BLUR,
                           QVariant::fromValue(pair));
    } else {
        view_->setFlags(Qt::Window | Qt::WindowDoesNotAcceptFocus |
                        Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint |
                        Qt::BypassWindowManagerHint);
    }

    view_->setGeometry(calculateInitialGeometry());
    setViewOpacity();

    connectSignals();
}

QRect VirtualKeyboardView::calculateInitialGeometry() {
    auto geo = geometry();
    auto screenHeight = getScreenRelativeHeight();

    int normalizedY = std::min(screenHeight, geo.y() + geo.height());

    return QRect(geo.x(), normalizedY, geo.width(), geo.height());
}

int VirtualKeyboardView::getScreenHeight() {
    return ScreenManager::getPrimaryScreenSize().height();
}

int VirtualKeyboardView::getScreenRelativeHeight() {
    auto screenRect = screenGeometry();
    return screenRect.y() + screenRect.height();
}

void VirtualKeyboardView::connectSignals() {
    auto *rootObject = view_->rootObject();

    connect(this, SIGNAL(updateCandidateArea(const QVariant &, int)),
            rootObject, SIGNAL(qmlUpdateCandidateList(QVariant, int)));
    connect(this, SIGNAL(imDeactivated()), rootObject,
            SIGNAL(qmlImDeactivated()));
}

void VirtualKeyboardView::destroyView() {
    if (view_ == nullptr) {
        return;
    }

    if (view_->isVisible()) {
        view_->hide();
    }

    if (!VirtualKeyboardSettings::getInstance().isPreloadViewEnabled()) {
        view_.release()->deleteLater();
    }
}

void VirtualKeyboardView::emitContentGeometrySignals() {
    emit contentHeightChanged();
    emit contentWidthChanged();
}

int VirtualKeyboardView::getContentWidth() {
    return getCurrentGeometryManager().getViewContentWidth();
}

int VirtualKeyboardView::getContentHeight() {
    return getCurrentGeometryManager().getViewContentHeight();
}

void VirtualKeyboardView::setViewOpacity() {
    if (VirtualKeyboardSettings::getInstance().isAnimationEnabled()) {
        view_->setOpacity(isFloatMode() ? 0.0f : 1.0f);
    }
}

GeometryManager &VirtualKeyboardView::getCurrentGeometryManager() const {
    if (isFloatMode()) {
        return *floatGeometryManager_;
    } else {
        return *expansionGeometryManager_;
    }
}
