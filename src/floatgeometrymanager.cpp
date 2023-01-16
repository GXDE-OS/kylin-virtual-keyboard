#include "floatgeometrymanager.h"
#include <QGuiApplication>
#include <QScreen>

FloatGeometryManager::FloatGeometryManager(VirtualKeyboardManager* virtualKeyboardManager, QObject *parent)
    : GeometryManager{virtualKeyboardManager, parent}
{
    position_ = loadVirtualKeyboardPosition();
}

void FloatGeometryManager::moveVirtualKeyboardByOffset(int offsetX, int offsetY)
{
    moveVirtualKeyboard(offsetX + position_.x(), offsetY + position_.y());
}

QPoint FloatGeometryManager::calculateNormalizedPosition(const QPoint& position) const
{
    int newX = 0;
    int newY = 0;
    QSize viewPortSize = QGuiApplication::primaryScreen()->geometry().size();
    
    if(position.x() > 0 && position.x() + size_.width() < viewPortSize.width()) {
        newX = position.x();
    }
    else if(position.x() <= 0) {
        newX = 0;
    }
    else {
        newX = viewPortSize.width() - size_.width();
    }
    
    if(position.y() > 0 && position.y() + size_.height() < viewPortSize.height()) {
        newY = position.y();
    }
    else if(position.y() <= 0) {
        newY = 0;
    }
    else {
        newY = viewPortSize.height() - size_.height();
    }
            
    return QPoint(newX, newY);
}

QPoint FloatGeometryManager::calculateVirtualKeyboardPosition()
{
    if(position_ != QPoint(0, 0)) {
        return position_;
    }
    
    QSize viewPortSize = QGuiApplication::primaryScreen()->geometry().size();
    int x = (viewPortSize.width() - size_.width()) / 2;
    int y = 0;
    y = viewPortSize.height() - 56 - size_.height();
    position_.setX(x);
    position_.setY(y);
    
    saveVirtualKeyboardPosition();
    return position_;
}

QSize FloatGeometryManager::calculateVirtualKeyboardSize()
{
    if(size_ != QSize(0, 0)) {
        return size_;
    }
    
    QSize viewPortSize = QGuiApplication::primaryScreen()->geometry().size();
    int width = viewPortSize.width() * virtualKeyboardWidthRatio_;
    int height = 0;
    if(viewPortSize.width() > viewPortSize.height()) {
        height = width * virtualKeyboardAspectRatio_;
    }
    else {
        height = viewPortSize.height() * virtualKeyboardWidthRatio_ * virtualKeyboardAspectRatio_;
    }
    size_.setWidth(width);
    size_.setHeight(height);
    return size_;
}

void FloatGeometryManager::saveVirtualKeyboardPosition()
{
    
}

QPoint FloatGeometryManager::loadVirtualKeyboardPosition()
{
    return position_;
}

void FloatGeometryManager::moveVirtualKeyboard(int x, int y)
{
    position_ = calculateNormalizedPosition(QPoint(x, y));
    emit virtualKeyboardMoved(position_.x(), position_.y());
    saveVirtualKeyboardPosition();
}


