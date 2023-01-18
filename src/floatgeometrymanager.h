#ifndef FLOATGEOMETRYMANAGER_H
#define FLOATGEOMETRYMANAGER_H

#include "geometrymanager.h"

class FloatGeometryManager : public GeometryManager {
    Q_OBJECT
public:
    explicit FloatGeometryManager(QObject *parent = nullptr);
    ~FloatGeometryManager() override;

public slots:
    void moveBy(int offsetX, int offsetY);
    void endDrag();

private:
    float getVirtualKeyboardWidthRatio() const override;
    float getVirtualKeyboardHeightRatio() const override;
    int calculateNormalizedX(int positionX) const;
    int calculateNormalizedY(int positionY) const;
    QPoint calculateNormalizedPosition(const QPoint &position) const;
    QPoint calculateVirtualKeyboardPosition() override;
    QMap<QString, QVariant> getVirtualKeyboardCurrentPositionRatioMap() const;
    QMap<QString, QVariant> getVirtualKeyboardDefaultPositionRatioMap() const;
    void saveVirtualKeyboardPosition();
    void loadVirtualKeyboardPosition();

    void moveVirtualKeyboard(const QPoint &targetPoint);

    constexpr static int distanceToBottom = 56;

    QPoint position_ = {0, 0};

    static QString virtualKeyboardPositionRatioMapKey;
    static QString virtualKeyboardPositionRatioXKey;
    static QString virtualKeyboardPositionRatioYKey;
};

#endif // FLOATGEOMETRYMANAGER_H
