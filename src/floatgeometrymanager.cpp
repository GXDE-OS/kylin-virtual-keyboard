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

FloatGeometryManager::FloatGeometryManager(
    VirtualKeyboardManager *virtualKeyboardManager, QObject *parent)
    : GeometryManager{virtualKeyboardManager, parent} {
    loadVirtualKeyboardPosition();
}

FloatGeometryManager::~FloatGeometryManager() { saveVirtualKeyboardPosition(); }

void FloatGeometryManager::moveVirtualKeyboardByOffset(int offsetX,
                                                       int offsetY) {
    moveVirtualKeyboard(position_.x() + offsetX, position_.y() + offsetY);
}

float FloatGeometryManager::getVirtualKeyboardWidthRatio() const {
    return 1458.0 / 1620.0;
}

float FloatGeometryManager::getVirtualKeyboardHeightRatio() const {
    return 548.0 / 1620.0;
}

QPoint FloatGeometryManager::calculateNormalizedPosition(
    const QPoint &position) const {
    int newX = 0;
    int newY = 0;
    QSize viewPortSize = QGuiApplication::primaryScreen()->geometry().size();
    auto virtualKeyboardSize = calculateVirtualKeyboardSize();
    if (position.x() > 0 &&
        position.x() + virtualKeyboardSize.width() < viewPortSize.width()) {
        newX = position.x();
    } else if (position.x() <= 0) {
        newX = 0;
    } else {
        newX = viewPortSize.width() - virtualKeyboardSize.width();
    }

    if (position.y() > 0 &&
        position.y() + virtualKeyboardSize.height() < viewPortSize.height()) {
        newY = position.y();
    } else if (position.y() <= 0) {
        newY = 0;
    } else {
        newY = viewPortSize.height() - virtualKeyboardSize.height();
    }

    return QPoint(newX, newY);
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

void FloatGeometryManager::moveVirtualKeyboard(int x, int y) {
    position_ = calculateNormalizedPosition(QPoint(x, y));
    emit virtualKeyboardMoved(position_.x(), position_.y());
    saveVirtualKeyboardPosition();
}
