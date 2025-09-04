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

#include "floatgeometrymanager.h"
#include <QMap>
#include "localsettings/localsettings.h"
#include "screenmanager.h"

// static
const QString FloatGeometryManager::floatGeometryGroup = "floatGeometry";
// static
const QString FloatGeometryManager::marginRatioMapKey = "marginRatioMap";
// static
const QString FloatGeometryManager::leftMarginRatioKey = "leftMarginRatio";
// static
const QString FloatGeometryManager::topMarginRatioKey = "topMarginRatio";

const QString FloatGeometryManager::lastPositionMapKey = "lastPositionMap";

const QString FloatGeometryManager::lastPositionXKey = "lastPositionX";

const QString FloatGeometryManager::lastPositionYKey = "lastPositionY";

const int FloatGeometryManager::defaultCoordinate = -99999999;

FloatGeometryManager::FloatGeometryManager(std::unique_ptr<Strategy> strategy,
                                           LocalSettings &viewSettings)
    : FloatGeometryManager(std::move(strategy), viewSettings, Scaler()) {}

FloatGeometryManager::FloatGeometryManager(std::unique_ptr<Strategy> strategy,
                                           LocalSettings &viewSettings,
                                           Scaler &&scaler)
    : GeometryManager(std::move(scaler)), strategy_(std::move(strategy)),
      viewSettings_(viewSettings) {
    loadLastPostionMap();
    loadMarginRatioMap();

    ScreenManager::screenRemoved([this]() {
        KVKBD_WARN("--- [screen removed] ---");
        const auto *screenAt = QGuiApplication::screenAt(currentPosition_);
        if (!screenAt) {
            KVKBD_WARN("screen removed, will reset view to primary screen!");
            resetParameters();
        }
    });
}

FloatGeometryManager::~FloatGeometryManager() {
}

void FloatGeometryManager::moveBy(int offsetX, int offsetY) {
    const QPoint offset(offsetX, offsetY);
    const auto currentPosition = calculateCurrentPosition();
    updateCurrentPostion(currentPosition);
    moveView(QPoint(currentPosition + offset));
}

void FloatGeometryManager::endDrag() {
    const QPoint currentPosition = calculateCurrentPosition();
    const auto viewRect = QRect(currentPosition, calculateViewSize());
    const auto adjustedPosition = adjustToScreenEdges(viewRect).topLeft();
    if (adjustedPosition != currentPosition) {
        updateCurrentPostion(adjustedPosition);
        moveView(adjustedPosition);
    }
    saveMarginRatioMap();
    saveLastPostionMap();
    updateGeometry();
}

int FloatGeometryManager::calculateViewWidth() const {
    return strategy_->getViewWidth(getScreenGeometry());
}

int FloatGeometryManager::calculateViewHeight() const {
    return strategy_->getViewHeight(getScreenGeometry());
}

QRect FloatGeometryManager::getScreenGeometry() const {
    return calculateProbableScreenGeometry();
}

QRect FloatGeometryManager::calculateProbableScreenGeometry() const {
    const auto *screenAt = QGuiApplication::screenAt(currentPosition_);
    if (screenAt) {
        KVKBD_DEBUG("using screenAt:{}", screenAt->name().toStdString());
        return screenAt->geometry();
    } else {
        KVKBD_DEBUG("using primary screen");
        return ScreenManager::getPrimaryScreenGeometry();
    }
}

QPoint FloatGeometryManager::calculateNormalizedPosition(
    const QPoint &position) const {
    const auto viewRect = QRect(position, calculateViewSize());
    const auto adjustedPosition = adjustToScreenEdges(viewRect).topLeft();
    return adjustedPosition;
}

QPoint FloatGeometryManager::calculateCurrentPosition() const {
    const auto currentPosition =
        calculatePositionFromRatio(leftMarginRatio_, topMarginRatio_);
    return currentPosition;
}

QPoint
FloatGeometryManager::calculatePositionFromRatio(float leftMarginRatio,
                                                 float topMarginRatio) const {
    const QSize marginSize = calculateMarginSize();

    return QPoint(marginSize.width() * leftMarginRatio,
                  marginSize.height() * topMarginRatio);
}

QPoint FloatGeometryManager::calculateNormalizedPositionFromRatio(
    float leftMarginRatio, float topMarginRatio) const {
    return calculateNormalizedPosition(
        calculatePositionFromRatio(leftMarginRatio, topMarginRatio));
}

QPoint FloatGeometryManager::calculateViewPosition() const {
    return calculateNormalizedPosition(calculateCurrentPosition());
}

QSize FloatGeometryManager::calculateMarginSize() const {
    const auto viewPortRect = calculateProbableScreenGeometry();
    const auto viewSize = calculateViewSize();

    const int horizontalMargin =
        viewPortRect.left() + viewPortRect.width() - viewSize.width();
    const int verticalMargin =
        viewPortRect.top() + viewPortRect.height() - viewSize.height();

    return QSize(horizontalMargin, verticalMargin);
}

QMap<QString, QVariant> FloatGeometryManager::getMarginRatioMap() const {
    QMap<QString, QVariant> marginRatioMap = {
        {leftMarginRatioKey, leftMarginRatio_},
        {topMarginRatioKey, topMarginRatio_}};

    return marginRatioMap;
}

QMap<QString, QVariant> FloatGeometryManager::getLastPositionMap() const {
    QMap<QString, QVariant> lastPositionMap = {
        {lastPositionXKey, currentPosition_.x()},
        {lastPositionYKey, currentPosition_.y()}};

    return lastPositionMap;
}

float FloatGeometryManager::calculateLeftMarginRatio(float leftMargin) const {
    return leftMargin / calculateMarginSize().width();
}

float FloatGeometryManager::calculateTopMarginRatio(float topMargin) const {
    return topMargin / calculateMarginSize().height();
}

void FloatGeometryManager::updateMarginRatio(const QPoint &targetPosition) {
    leftMarginRatio_ = calculateLeftMarginRatio(targetPosition.x());
    topMarginRatio_ = calculateTopMarginRatio(targetPosition.y());
    KVKBD_DEBUG("leftMarginRatio_:{}, topMarginRatio_:{}", leftMarginRatio_,
                topMarginRatio_);
}

void FloatGeometryManager::updateCurrentPostion(const QPoint &position) {
    currentPosition_ = position;
}

void FloatGeometryManager::saveMarginRatioMap() {
    viewSettings_.setValue(floatGeometryGroup, marginRatioMapKey,
                           getMarginRatioMap());
}

void FloatGeometryManager::saveLastPostionMap() {
    viewSettings_.setValue(floatGeometryGroup, lastPositionMapKey,
                           getLastPositionMap());
}

QMap<QString, QVariant> FloatGeometryManager::getDefaultMarginRatioMap() const {
    const QRect viewPortGeo = ScreenManager::getPrimaryScreenGeometry();
    const auto viewSize = calculateViewSize();

    const int leftMargin =
        viewPortGeo.left() + viewPortGeo.width() -
        (viewSize.width() + strategy_->getDefaultRightMargin());
    const int topMargin =
        viewPortGeo.top() + viewPortGeo.height() -
        (viewSize.height() + strategy_->getDefaultBottomMargin());

    const float defaultLeftMarginRatio = calculateLeftMarginRatio(leftMargin);
    const float defaultTopMarginRatio = calculateTopMarginRatio(topMargin);

    QMap<QString, QVariant> viewDefaultMarginRatioMap = {
        {leftMarginRatioKey, defaultLeftMarginRatio},
        {topMarginRatioKey, defaultTopMarginRatio}};

    return viewDefaultMarginRatioMap;
}

QMap<QString, QVariant>
FloatGeometryManager::getDefaultLastPositionMap() const {
    QMap<QString, QVariant> viewDefaultLastPositionMap = {
        {lastPositionXKey, defaultCoordinate},
        {lastPositionYKey, defaultCoordinate}};

    return viewDefaultLastPositionMap;
}

void FloatGeometryManager::loadMarginRatioMap() {
    const auto marginRatioMap =
        viewSettings_
            .getValue(floatGeometryGroup, marginRatioMapKey,
                      getDefaultMarginRatioMap())
            .toMap();

    const float leftMarginRatio = marginRatioMap[leftMarginRatioKey].toFloat();
    const float topMarginRatio = marginRatioMap[topMarginRatioKey].toFloat();
    KVKBD_DEBUG("leftMarginRatio_:{}, topMarginRatio_:{}", leftMarginRatio_,
                topMarginRatio_);
    updateMarginRatio(
        calculateNormalizedPositionFromRatio(leftMarginRatio, topMarginRatio));
}

void FloatGeometryManager::loadLastPostionMap() {
    KVKBD_DEBUG("floatGeometryGroup:{}", floatGeometryGroup.toStdString());
    const auto lastPositionMap =
        viewSettings_
            .getValue(floatGeometryGroup, lastPositionMapKey,
                      getDefaultLastPositionMap())
            .toMap();

    const auto lastPostionX = lastPositionMap[lastPositionXKey].toInt();
    const auto lastPostionY = lastPositionMap[lastPositionYKey].toInt();

    updateCurrentPostion(QPoint(lastPostionX, lastPostionY));
}

void FloatGeometryManager::resetParameters() {
    const auto lastPositionMap = getDefaultLastPositionMap();
    const auto lastPostionX = lastPositionMap[lastPositionXKey].toInt();
    const auto lastPostionY = lastPositionMap[lastPositionYKey].toInt();

    auto defaultPosition = QPoint(lastPostionX, lastPostionY);
    updateCurrentPostion(defaultPosition);
    saveLastPostionMap();

    const auto marginRatioMap = getDefaultMarginRatioMap();
    const float leftMarginRatio = marginRatioMap[leftMarginRatioKey].toFloat();
    const float topMarginRatio = marginRatioMap[topMarginRatioKey].toFloat();

    leftMarginRatio_ = leftMarginRatio;
    topMarginRatio_ = topMarginRatio;
    saveMarginRatioMap();

    KVKBD_WARN("leftMarginRatio_:{}, topMarginRatio_:{}", leftMarginRatio,
               topMarginRatio);
}

void FloatGeometryManager::moveView(const QPoint &targetPoint) {
    updateMarginRatio(targetPoint);

    const QPoint currentPosition = calculateCurrentPosition();
    emit viewMoved(currentPosition.x(), currentPosition.y());
}

QRect FloatGeometryManager::adjustToScreenEdges(const QRect &windowRect) const {
    QScreen *targetScreen = QGuiApplication::screenAt(windowRect.center());

    if (!targetScreen) {
        for (QScreen *screen : QGuiApplication::screens()) {
            if (screen->geometry().intersects(windowRect)) {
                targetScreen = screen;
                break;
            }
        }
    }

    if (!targetScreen) {
        QPoint windowCenter = windowRect.center();
        qreal minDistance = std::numeric_limits<qreal>::max();
        for (QScreen *screen : QGuiApplication::screens()) {
            QRect screenGeo = screen->geometry();
            QPoint screenCenter = screenGeo.center();
            qreal distance = QLineF(windowCenter, screenCenter).length();
            if (distance < minDistance) {
                minDistance = distance;
                targetScreen = screen;
            }
        }
    }

    if (!targetScreen) {
        targetScreen = QGuiApplication::primaryScreen();
    }

    QRect screenGeo = targetScreen->geometry();
    QRect adjusted = windowRect;

    if (adjusted.left() < screenGeo.left())
        adjusted.moveLeft(screenGeo.left());
    if (adjusted.right() > screenGeo.right())
        adjusted.moveRight(screenGeo.right());
    if (adjusted.top() < screenGeo.top())
        adjusted.moveTop(screenGeo.top());
    if (adjusted.bottom() > screenGeo.bottom())
        adjusted.moveBottom(screenGeo.bottom());

    return adjusted;
}
