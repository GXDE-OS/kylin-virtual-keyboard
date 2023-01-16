#include "floatgeometrymanager.h"
#include <QGuiApplication>
#include <QScreen>

FloatGeometryManager::FloatGeometryManager(
    VirtualKeyboardManager *virtualKeyboardManager, QObject *parent)
    : GeometryManager{virtualKeyboardManager, parent} {
    position_ = loadVirtualKeyboardPosition();
}

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
    if (position_ != QPoint(0, 0)) {
        return position_;
    }

    QSize viewPortSize = QGuiApplication::primaryScreen()->geometry().size();
    auto virtualKeyboardSize = calculateVirtualKeyboardSize();
    int x = (viewPortSize.width() - virtualKeyboardSize.width()) / 2;
    int y = viewPortSize.height() -
            (virtualKeyboardSize.height() + distanceToBottom);
    position_.setX(x);
    position_.setY(y);

    saveVirtualKeyboardPosition();
    return position_;
}

void FloatGeometryManager::saveVirtualKeyboardPosition() {}

QPoint FloatGeometryManager::loadVirtualKeyboardPosition() { return position_; }

void FloatGeometryManager::moveVirtualKeyboard(int x, int y) {
    position_ = calculateNormalizedPosition(QPoint(x, y));
    emit virtualKeyboardMoved(position_.x(), position_.y());
    saveVirtualKeyboardPosition();
}
