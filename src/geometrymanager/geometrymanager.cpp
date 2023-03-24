#include "geometrymanager.h"

#include "screenmanager.h"

GeometryManager::GeometryManager() : QObject() {}

QSize GeometryManager::calculateViewSize() const {
    return QSize(calculateViewWidth(), calculateViewHeight());
}

void GeometryManager::updateGeometry() {
    QPoint position = calculateViewPosition();
    emit viewMoved(position.x(), position.y());

    QSize size = calculateViewSize();
    emit viewResized(size.width(), size.height());
}
