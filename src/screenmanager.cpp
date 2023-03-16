#include "screenmanager.h"

#include <QGuiApplication>
#include <QScreen>

// static
QRect ScreenManager::getPrimaryScreenGeometry() {
    return QGuiApplication::primaryScreen()->geometry();
}

// static
QSize ScreenManager::getPrimaryScreenSize() {
    return getPrimaryScreenGeometry().size();
}
