#include "appinputareamanager.h"
#include <KWindowSystem>

AppInputAreaManager::AppInputAreaManager(QObject *parent) : QObject(parent) {}

void AppInputAreaManager::raiseInputArea(QQuickView *view,
                                         const QRect &virtualKeyboardRect) {
    //使用KWin接口调整工作区域，仅在X11下有效
    KWindowSystem::setExtendedStrut(
        view->winId(), 0, 0, 0, 0, 0, 0, 0, 0, 0, virtualKeyboardRect.height(),
        virtualKeyboardRect.x(), virtualKeyboardRect.width() - 1);
}
