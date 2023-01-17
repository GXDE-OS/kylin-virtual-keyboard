#include "geometrymanager.h"

#include <QGuiApplication>
#include <QScreen>

GeometryManager::GeometryManager(QObject *parent) : QObject(parent) {}

int GeometryManager::calculateVirtualKeyboardWidth() const {
    QSize viewPortSize = QGuiApplication::primaryScreen()->geometry().size();

    return viewPortSize.width() * getVirtualKeyboardWidthRatio();
}

int GeometryManager::calculateVirtualKeyboardHeight() const {
    QSize viewPortSize = QGuiApplication::primaryScreen()->geometry().size();

    if (viewPortSize.width() > viewPortSize.height()) {
        return viewPortSize.width() * getVirtualKeyboardHeightRatio();
    } else {
        // 竖屏情况下，从屏幕左上角开始算起，以当前屏幕宽度为测量基准，
        // 在该范围里面的部分可以正常刷新；在该范围外面部分会出现重影。
        // 因此，需要将虚拟键盘的高度设置为屏幕宽高差值+1，使其可以正确
        // 刷新qml内容, 防止旋转到横屏后出现重影
        // TODO(linyuxuan): 找出bug成因，彻底解决该问题
        return viewPortSize.height() - viewPortSize.width() + 1;
    }
}

QSize GeometryManager::calculateVirtualKeyboardSize() const {
    return QSize(calculateVirtualKeyboardWidth(),
                 calculateVirtualKeyboardHeight());
}

void GeometryManager::updateGeometry() {
    QPoint position = calculateVirtualKeyboardPosition();
    emit virtualKeyboardMoved(position.x(), position.y());

    QSize size = calculateVirtualKeyboardSize();
    emit virtualKeyboardResized(size.width(), size.height());
}
