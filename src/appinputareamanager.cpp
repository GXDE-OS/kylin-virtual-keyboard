#include "appinputareamanager.h"
#include <KWindowSystem>

AppInputAreaManager::AppInputAreaManager(QObject *parent)
    : QObject(parent), dummyWidget_(nullptr), oneshotTimer_(nullptr) {
    KWindowSystem::setType(dummyWidget_.winId(), NET::Dock);
    oneshotTimer_.setSingleShot(true);
}

void AppInputAreaManager::raiseInputArea(const QRect &virtualKeyboardRect) {
    dummyWidget_.setGeometry(virtualKeyboardRect);
    dummyWidget_.show();

    WId winId = dummyWidget_.winId();
    QObject::connect(
        &oneshotTimer_, &QTimer::timeout, this, [winId, virtualKeyboardRect]() {
            // 使用KWin接口调整工作区域，仅在X11下有效
            // 该接口对全屏应用无效
            KWindowSystem::setExtendedStrut(
                winId, 0, 0, 0, 0, 0, 0, 0, 0, 0, virtualKeyboardRect.height(),
                virtualKeyboardRect.x(), virtualKeyboardRect.width() - 1);
        });
    oneshotTimer_.start(SHOW_DELAY_TIME);
}

void AppInputAreaManager::fallInputArea() {
    KWindowSystem::setExtendedStrut(dummyWidget_.winId(), 0, 0, 0, 0, 0, 0, 0,
                                    0, 0, 0, 0, 0);
    dummyWidget_.hide();
    oneshotTimer_.stop();
}
