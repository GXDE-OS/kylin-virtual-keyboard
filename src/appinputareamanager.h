#ifndef APPINPUTAREAMANAGER_H
#define APPINPUTAREAMANAGER_H

#include <QObject>
#include <QQuickView>
#include <QRect>

class AppInputAreaManager : public QObject {
    Q_OBJECT

public:
    explicit AppInputAreaManager(QObject *parent = nullptr);
    ~AppInputAreaManager() = default;
    static void raiseInputArea(QQuickView *view,
                               const QRect &virtualKeyboardRect);
};

#endif // APPINPUTAREAMANAGER_H
