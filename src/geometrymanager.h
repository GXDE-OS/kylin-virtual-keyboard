#ifndef GEOMETRYMANAGER_H
#define GEOMETRYMANAGER_H

#include <QObject>
#include <QPoint>
#include <QSize>

class VirtualKeyboardManager;

class GeometryManager : public QObject {
    Q_OBJECT
public:
    ~GeometryManager() override = default;

public slots:
    void updateGeometry();

signals:
    void virtualKeyboardMoved(int x, int y);
    void virtualKeyboardResized(int width, int height);

protected:
    GeometryManager();

    int calculateVirtualKeyboardWidth() const;
    int calculateVirtualKeyboardHeight() const;
    QSize calculateVirtualKeyboardSize() const;

    static QRect getPrimaryScreenGeometry();

private:
    virtual float getVirtualKeyboardWidthRatio() const = 0;
    virtual float getVirtualKeyboardHeightRatio() const = 0;

    virtual QPoint calculateVirtualKeyboardPosition() = 0;
};

#endif // GEOMETRYMANAGER_H
