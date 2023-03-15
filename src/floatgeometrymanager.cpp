#include "floatgeometrymanager.h"

#include <QGuiApplication>
#include <QMap>
#include <QScreen>

#include "virtualkeyboardsettings.h"

// static
const QString FloatGeometryManager::floatGeometryGroup = "floatGeometry";
// static
const QString FloatGeometryManager::marginRatioMapKey = "marginRatioMap";
// static
const QString FloatGeometryManager::leftMarginRatioKey = "leftMarginRatio";
// static
const QString FloatGeometryManager::topMarginRatioKey = "topMarginRatio";

FloatGeometryManager::FloatGeometryManager() : GeometryManager() {
    loadMarginRatioMap();
}

FloatGeometryManager::~FloatGeometryManager() { saveMarginRatioMap(); }

void FloatGeometryManager::moveBy(int offsetX, int offsetY) {
    const QPoint offset(offsetX, offsetY);

    moveVirtualKeyboard(QPoint(calculateCurrentPosition() + offset));
}

void FloatGeometryManager::endDrag() {
    const QPoint currentPosition = calculateCurrentPosition();
    const QPoint normalizedPosition =
        calculateNormalizedPosition(currentPosition);
    if (normalizedPosition != currentPosition) {
        moveVirtualKeyboard(normalizedPosition);
    }
}

float FloatGeometryManager::getVirtualKeyboardWidthRatio() const {
    return 1458.0 / 1620.0;
}

float FloatGeometryManager::getVirtualKeyboardHeightRatio() const {
    return 548.0 / 1620.0;
}

int FloatGeometryManager::calculateNormalizedX(int positionX) const {
    const auto geometry = getPrimaryScreenGeometry();

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
    const auto geometry = getPrimaryScreenGeometry();

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

QPoint FloatGeometryManager::calculateCurrentPosition() const {
    return calculatePositionFromRatio(leftMarginRatio_, topMarginRatio_);
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

QPoint FloatGeometryManager::calculateVirtualKeyboardPosition() {
    return calculateNormalizedPosition(calculateCurrentPosition());
}

QSize FloatGeometryManager::calculateMarginSize() const {
    const QSize viewPortSize = getPrimaryScreenGeometry().size();
    const auto virtualKeyboardSize = calculateVirtualKeyboardSize();

    const int horizontalMargin =
        viewPortSize.width() - virtualKeyboardSize.width();
    const int verticalMargin =
        viewPortSize.height() - virtualKeyboardSize.height();

    return QSize(horizontalMargin, verticalMargin);
}

QMap<QString, QVariant> FloatGeometryManager::getMarginRatioMap() const {
    QMap<QString, QVariant> marginRatioMap = {
        {leftMarginRatioKey, leftMarginRatio_},
        {topMarginRatioKey, topMarginRatio_}};

    return marginRatioMap;
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
}

void FloatGeometryManager::saveMarginRatioMap() {
    VirtualKeyboardSettings::getInstance().setValue(
        floatGeometryGroup, marginRatioMapKey, getMarginRatioMap());
}

QMap<QString, QVariant> FloatGeometryManager::getDefaultMarginRatioMap() const {
    const QSize viewPortSize = getPrimaryScreenGeometry().size();
    const auto virtualKeyboardSize = calculateVirtualKeyboardSize();

    const int leftMargin =
        (viewPortSize.width() - virtualKeyboardSize.width()) / 2;
    const int defaultBottomMargin =
        viewPortSize.height() * defaultBottomMarginRatio;
    const int topMargin = viewPortSize.height() -
                          (virtualKeyboardSize.height() + defaultBottomMargin);

    const float defaultLeftMarginRatio = calculateLeftMarginRatio(leftMargin);
    const float defaultTopMarginRatio = calculateTopMarginRatio(topMargin);

    QMap<QString, QVariant> virtualKeyboardDefaultMarginRatioMap = {
        {leftMarginRatioKey, defaultLeftMarginRatio},
        {topMarginRatioKey, defaultTopMarginRatio}};

    return virtualKeyboardDefaultMarginRatioMap;
}

void FloatGeometryManager::loadMarginRatioMap() {
    const auto marginRatioMap =
        VirtualKeyboardSettings::getInstance()
            .getValue(floatGeometryGroup, marginRatioMapKey,
                      getDefaultMarginRatioMap())
            .toMap();

    const float leftMarginRatio = marginRatioMap[leftMarginRatioKey].toFloat();
    const float topMarginRatio = marginRatioMap[topMarginRatioKey].toFloat();

    updateMarginRatio(
        calculateNormalizedPositionFromRatio(leftMarginRatio, topMarginRatio));
}

void FloatGeometryManager::moveVirtualKeyboard(const QPoint &targetPoint) {
    updateMarginRatio(targetPoint);

    const QPoint currentPosition = calculateCurrentPosition();
    emit virtualKeyboardMoved(currentPosition.x(), currentPosition.y());

    saveMarginRatioMap();
}
