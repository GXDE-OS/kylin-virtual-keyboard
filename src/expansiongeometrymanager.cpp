#include "expansiongeometrymanager.h"
#include <QGuiApplication>
#include <QScreen>

ExpansionGeometryManager::ExpansionGeometryManager() : GeometryManager() {}

float ExpansionGeometryManager::getViewWidthRatio() const { return 1.0f; }

float ExpansionGeometryManager::getViewHeightRatio() const {
    return 512.0 / 1620.0;
}

QPoint ExpansionGeometryManager::calculateViewPosition() {
    QSize viewPortSize = getPrimaryScreenGeometry().size();

    return QPoint(0, viewPortSize.height() - calculateViewHeight());
}
