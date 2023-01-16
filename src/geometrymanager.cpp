#include "geometrymanager.h"

GeometryManager::GeometryManager(VirtualKeyboardManager *virtualKeyboardManager,
                                 QObject *parent)
    : QObject(parent), virtualKeyboardManager_(virtualKeyboardManager) {}
void GeometryManager::updateGeometry() {
    QSize size = calculateVirtualKeyboardSize();
    QPoint position = calculateVirtualKeyboardPosition();
    emit virtualKeyboardMoved(position.x(), position.y());
    emit virtualKeyboardResized(size.width(), size.height());
}

QPoint GeometryManager::calculateVirtualKeyboardPosition() {
    return QPoint(0, 0);
}

QSize GeometryManager::calculateVirtualKeyboardSize() { return QSize(0, 0); }
