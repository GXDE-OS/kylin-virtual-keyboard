#ifndef GEOMETRYMANAGER_H
#define GEOMETRYMANAGER_H

#include <QObject>
#include <QPoint>
#include <QSize>

class VirtualKeyboardManager;

class GeometryManager : public QObject {
    Q_OBJECT
public:
    virtual ~GeometryManager() = default;

public slots:
    void updateGeometry();

signals:
    void virtualKeyboardMoved(int x, int y);
    void virtualKeyboardResized(int width, int height);

protected:
    explicit GeometryManager(QObject *parent = nullptr);

    int calculateVirtualKeyboardWidth() const;
    int calculateVirtualKeyboardHeight() const;
    QSize calculateVirtualKeyboardSize() const;

private:
    virtual float getVirtualKeyboardWidthRatio() const = 0;
    virtual float getVirtualKeyboardHeightRatio() const = 0;

    virtual QPoint calculateVirtualKeyboardPosition() = 0;
};

#endif // GEOMETRYMANAGER_H
