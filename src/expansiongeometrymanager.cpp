#include "expansiongeometrymanager.h"
#include <QGuiApplication>
#include <QScreen>

ExpansionGeometryManager::ExpansionGeometryManager() : GeometryManager() {}

float ExpansionGeometryManager::getVirtualKeyboardWidthRatio() const {
    return 1.0f;
}

float ExpansionGeometryManager::getVirtualKeyboardHeightRatio() const {
    return 512.0 / 1620.0;
}

QPoint ExpansionGeometryManager::calculateVirtualKeyboardPosition() {
    QSize viewPortSize = getPrimaryScreenGeometry().size();

    return QPoint(0, viewPortSize.height() - calculateVirtualKeyboardHeight());
}
