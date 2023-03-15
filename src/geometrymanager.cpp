#include "geometrymanager.h"

#include <QGuiApplication>
#include <QScreen>

GeometryManager::GeometryManager() : QObject() {}

int GeometryManager::calculateViewWidth() const {
    QSize viewPortSize = getPrimaryScreenGeometry().size();

    return viewPortSize.width() * getViewWidthRatio();
}

int GeometryManager::calculateViewHeight() const {
    QSize viewPortSize = getPrimaryScreenGeometry().size();

    if (viewPortSize.width() > viewPortSize.height()) {
        return viewPortSize.width() * getViewHeightRatio();
    } else {
        // 竖屏情况下，从屏幕左上角开始算起，以当前屏幕宽度为测量基准，
        // 在该范围里面的部分可以正常刷新；在该范围外面部分会出现重影。
        // 因此，需要将虚拟键盘的高度设置为屏幕宽高差值+1，将其作为虚拟
        // 键盘的最小高度,从而确保虚拟键盘可以正确刷新qml内容, 防止旋转
        // 到横屏后出现重影
        // TODO(linyuxuan): 找出bug成因，彻底解决该问题
        const int leastHeight =
            viewPortSize.height() - viewPortSize.width() + 1;
        const int requiredHeight = viewPortSize.height() * getViewHeightRatio();
        return std::max(leastHeight, requiredHeight);
    }
}

QSize GeometryManager::calculateViewSize() const {
    return QSize(calculateViewWidth(), calculateViewHeight());
}

// static
QRect GeometryManager::getPrimaryScreenGeometry() {
    return QGuiApplication::primaryScreen()->geometry();
}

void GeometryManager::updateGeometry() {
    QPoint position = calculateViewPosition();
    emit viewMoved(position.x(), position.y());

    QSize size = calculateViewSize();
    emit viewResized(size.width(), size.height());
}
