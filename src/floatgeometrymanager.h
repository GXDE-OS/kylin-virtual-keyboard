#ifndef FLOATGEOMETRYMANAGER_H
#define FLOATGEOMETRYMANAGER_H

#include "geometrymanager.h"

class FloatGeometryManager : public GeometryManager {
    Q_OBJECT
public:
    explicit FloatGeometryManager(
        VirtualKeyboardManager *virtualKeyboardManager,
        QObject *parent = nullptr);
    ~FloatGeometryManager() = default;

public slots:
    void moveVirtualKeyboardByOffset(int offsetX, int offsetY);

private:
    QPoint calculateNormalizedPosition(const QPoint &position) const;
    QPoint calculateVirtualKeyboardPosition() override;
    QSize calculateVirtualKeyboardSize() override;
    void saveVirtualKeyboardPosition();
    QPoint loadVirtualKeyboardPosition();
    void moveVirtualKeyboard(int x, int y);

    int distanceToBottom = 56;
    float virtualKeyboardWidthRatio_ = 1458.0 / 1620.0;
    float virtualKeyboardAspectRatio_ = 548.0 / 1458.0;
    QPoint position_ = {0, 0};
    QSize size_ = {0, 0};
};

#endif // FLOATGEOMETRYMANAGER_H
