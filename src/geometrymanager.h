#ifndef GEOMETRYMANAGER_H
#define GEOMETRYMANAGER_H

#include <QObject>
#include <QPoint>
#include <QSize>

class VirtualKeyboardManager;

class GeometryManager : public QObject {
    Q_OBJECT
public:
    explicit GeometryManager(VirtualKeyboardManager *virtualKeyboardManager,
                             QObject *parent = nullptr);
    ~GeometryManager() = default;

public slots:
    void updateGeometry();

signals:
    void virtualKeyboardMoved(int x, int y);
    void virtualKeyboardResized(int width, int height);

private:
    virtual QPoint calculateVirtualKeyboardPosition();
    virtual QSize calculateVirtualKeyboardSize();

protected:
    VirtualKeyboardManager *virtualKeyboardManager_ = nullptr;
};

#endif // GEOMETRYMANAGER_H
