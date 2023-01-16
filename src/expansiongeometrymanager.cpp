#include "expansiongeometrymanager.h"
#include <QGuiApplication>
#include <QScreen>

ExpansionGeometryManager::ExpansionGeometryManager(
    VirtualKeyboardManager *virtualKeyboardManager, QObject *parent)
    : GeometryManager{virtualKeyboardManager, parent} {}

float ExpansionGeometryManager::getVirtualKeyboardWidthRatio() const {
    return 1.0f;
}

float ExpansionGeometryManager::getVirtualKeyboardHeightRatio() const {
    return 512.0 / 1620.0;
}

QPoint ExpansionGeometryManager::calculateVirtualKeyboardPosition() {
    QSize viewPortSize = QGuiApplication::primaryScreen()->geometry().size();

    return QPoint(0, viewPortSize.height() - calculateVirtualKeyboardHeight());
}
