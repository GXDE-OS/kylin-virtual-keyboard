#include "expansiongeometrymanager.h"
#include <QGuiApplication>
#include <QScreen>

ExpansionGeometryManager::ExpansionGeometryManager(VirtualKeyboardManager* virtualKeyboardManager,
    QObject *parent): GeometryManager{virtualKeyboardManager, parent}
{
  
}

QPoint ExpansionGeometryManager::calculateVirtualKeyboardPosition()
{
    QSize viewPortSize = QGuiApplication::primaryScreen()->geometry().size();
    int x = 0;
    int y = 0;
    if(viewPortSize.width() > viewPortSize.height()) {
        y = viewPortSize.height() - viewPortSize.width() * virtualKeyboardAspectRatio_;
    }
    else {
        y = viewPortSize.height() - viewPortSize.height() * virtualKeyboardAspectRatio_;
    }
    return QPoint(x, y);
}

QSize ExpansionGeometryManager::calculateVirtualKeyboardSize()
{
    QSize viewPortSize = QGuiApplication::primaryScreen()->geometry().size();
    int width = viewPortSize.width();
    int height = 0;
    if(viewPortSize.width() > viewPortSize.height()) {
        height = width * virtualKeyboardAspectRatio_;
    }
    else {
        height = viewPortSize.height() * virtualKeyboardAspectRatio_;
    }
    return QSize(width, height);
}
