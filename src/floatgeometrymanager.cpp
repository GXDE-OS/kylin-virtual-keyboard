#include "floatgeometrymanager.h"

#include <QGuiApplication>
#include <QMap>
#include <QScreen>

#include "virtualkeyboardsettings.h"

// static
QString FloatGeometryManager::virtualKeyboardPositionRatioMapKey =
    "virtualKeyboardPositionRatioMap";
// static
QString FloatGeometryManager::virtualKeyboardPositionRatioXKey =
    "virtualKeyboardPositionRatioX";
// static
QString FloatGeometryManager::virtualKeyboardPositionRatioYKey =
    "virtualKeyboardPositionRatioY";

FloatGeometryManager::FloatGeometryManager(QObject *parent)
    : GeometryManager{parent} {
    loadVirtualKeyboardPosition();
}

FloatGeometryManager::~FloatGeometryManager() { saveVirtualKeyboardPosition(); }

void FloatGeometryManager::moveBy(int offsetX, int offsetY) {
    const QPoint offset(offsetX, offsetY);

    moveVirtualKeyboard(QPoint(position_ + offset));
}

void FloatGeometryManager::endDrag() {
    const QPoint normalizedPoint = calculateNormalizedPosition(position_);
    if (normalizedPoint != position_) {
        moveVirtualKeyboard(normalizedPoint);
    }
}

float FloatGeometryManager::getVirtualKeyboardWidthRatio() const {
    return 1458.0 / 1620.0;
}

float FloatGeometryManager::getVirtualKeyboardHeightRatio() const {
    return 548.0 / 1620.0;
}

int FloatGeometryManager::calculateNormalizedX(int positionX) const {
    const auto geometry = QGuiApplication::primaryScreen()->geometry();

    if (positionX < geometry.left()) {
        return geometry.left();
    }

    const auto virtualKeyboardWidth = calculateVirtualKeyboardWidth();
    if (positionX + virtualKeyboardWidth > geometry.right()) {
        return geometry.right() - virtualKeyboardWidth;
    }

    return positionX;
}

int FloatGeometryManager::calculateNormalizedY(int positionY) const {
    const auto geometry = QGuiApplication::primaryScreen()->geometry();

    if (positionY < geometry.top()) {
        return geometry.top();
    }

    const auto virtualKeyboardHeight = calculateVirtualKeyboardHeight();
    if (positionY + virtualKeyboardHeight > geometry.bottom()) {
        return geometry.bottom() - virtualKeyboardHeight;
    }

    return positionY;
}

QPoint FloatGeometryManager::calculateNormalizedPosition(
    const QPoint &position) const {
    return QPoint(calculateNormalizedX(position.x()),
                  calculateNormalizedY(position.y()));
}

QPoint FloatGeometryManager::calculateVirtualKeyboardPosition() {
    return position_;
}

QMap<QString, QVariant>
FloatGeometryManager::getVirtualKeyboardCurrentPositionRatioMap() const {
    const QSize viewPortSize =
        QGuiApplication::primaryScreen()->geometry().size();

    const float positionX = position_.x();
    const float positionY = position_.y();
    const float ratioX = positionX / viewPortSize.width();
    const float ratioY = positionY / viewPortSize.height();
    QMap<QString, QVariant> virtualKeyboardPositionRatioMap = {
        {virtualKeyboardPositionRatioXKey, ratioX},
        {virtualKeyboardPositionRatioYKey, ratioY}};

    return virtualKeyboardPositionRatioMap;
}

void FloatGeometryManager::saveVirtualKeyboardPosition() {
    VirtualKeyboardSettings::getInstance().setValue(
        virtualKeyboardPositionRatioMapKey,
        getVirtualKeyboardCurrentPositionRatioMap());
}

QMap<QString, QVariant>
FloatGeometryManager::getVirtualKeyboardDefaultPositionRatioMap() const {
    const QSize viewPortSize =
        QGuiApplication::primaryScreen()->geometry().size();
    const auto virtualKeyboardSize = calculateVirtualKeyboardSize();

    const float defaultPositionX =
        (viewPortSize.width() - virtualKeyboardSize.width()) / 2;
    const float defaultPositionY =
        viewPortSize.height() -
        (virtualKeyboardSize.height() + distanceToBottom);
    const float defaultRatioX = defaultPositionX / viewPortSize.width();
    const float defaultRatioY = defaultPositionY / viewPortSize.height();

    QMap<QString, QVariant> virtualKeyboardDefaultPositionRatioMap = {
        {virtualKeyboardPositionRatioXKey, defaultRatioX},
        {virtualKeyboardPositionRatioYKey, defaultRatioY}};

    return virtualKeyboardDefaultPositionRatioMap;
}

void FloatGeometryManager::loadVirtualKeyboardPosition() {
    const auto virtualKeyboardPositionRatioMap =
        VirtualKeyboardSettings::getInstance()
            .getValue(virtualKeyboardPositionRatioMapKey,
                      getVirtualKeyboardDefaultPositionRatioMap())
            .toMap();

    const QSize viewPortSize =
        QGuiApplication::primaryScreen()->geometry().size();

    const float ratioX =
        virtualKeyboardPositionRatioMap[virtualKeyboardPositionRatioXKey]
            .toFloat();
    const float ratioY =
        virtualKeyboardPositionRatioMap[virtualKeyboardPositionRatioYKey]
            .toFloat();

    const QPoint virtualKeyboardPosition(viewPortSize.width() * ratioX,
                                         viewPortSize.height() * ratioY);
    position_ = calculateNormalizedPosition(virtualKeyboardPosition);
}

void FloatGeometryManager::moveVirtualKeyboard(const QPoint &targetPoint) {
    position_ = targetPoint;
    emit virtualKeyboardMoved(position_.x(), position_.y());
    saveVirtualKeyboardPosition();
}
