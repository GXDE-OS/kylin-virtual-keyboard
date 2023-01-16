#ifndef FLOATGEOMETRYMANAGER_H
#define FLOATGEOMETRYMANAGER_H

#include "geometrymanager.h"

class FloatGeometryManager : public GeometryManager {
    Q_OBJECT
public:
    explicit FloatGeometryManager(
        VirtualKeyboardManager *virtualKeyboardManager,
        QObject *parent = nullptr);
    ~FloatGeometryManager() override = default;

public slots:
    void moveVirtualKeyboardByOffset(int offsetX, int offsetY);

private:
    float getVirtualKeyboardWidthRatio() const override;
    float getVirtualKeyboardHeightRatio() const override;
    QPoint calculateNormalizedPosition(const QPoint &position) const;
    QPoint calculateVirtualKeyboardPosition() override;
    void saveVirtualKeyboardPosition();
    QPoint loadVirtualKeyboardPosition();
    void moveVirtualKeyboard(int x, int y);

    constexpr static int distanceToBottom = 56;

    QPoint position_ = {0, 0};
};

#endif // FLOATGEOMETRYMANAGER_H
