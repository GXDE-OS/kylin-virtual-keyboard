#ifndef SCREENMANAGER_H
#define SCREENMANAGER_H

#include <QRect>
#include <QSize>

class ScreenManager {
public:
    ScreenManager() = default;
    ~ScreenManager() = default;

    static QRect getPrimaryScreenGeometry();
    static QSize getPrimaryScreenSize();
};

#endif // SCREENMANAGER_H
