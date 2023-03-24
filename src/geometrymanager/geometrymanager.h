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
    void viewMoved(int x, int y);
    void viewResized(int width, int height);

protected:
    GeometryManager();

    QSize calculateViewSize() const;

private:
    virtual int calculateViewWidth() const = 0;
    virtual int calculateViewHeight() const = 0;

    virtual QPoint calculateViewPosition() = 0;
};

#endif // GEOMETRYMANAGER_H
