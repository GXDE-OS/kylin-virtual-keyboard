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
    QPoint calculateVirtualKeyboardPosition() override;
    float getVirtualKeyboardWidthRatio() const override;
    float getVirtualKeyboardHeightRatio() const override;

    int calculateNormalizedX(int positionX) const;
    int calculateNormalizedY(int positionY) const;
    QPoint calculateNormalizedPosition(const QPoint &position) const;

    QPoint calculatePositionFromRatio(float leftMarginRatio,
                                      float topMarginRatio) const;
    QPoint calculateCurrentPosition() const;
    QPoint calculateNormalizedPositionFromRatio(float leftMarginRatio,
                                                float topMarginRatio) const;

    QSize calculateMarginSize() const;
    QMap<QString, QVariant> getMarginRatioMap() const;
    QMap<QString, QVariant> getDefaultMarginRatioMap() const;
    float calculateLeftMarginRatio(float leftMargin) const;
    float calculateTopMarginRatio(float topMargin) const;
    void updateMarginRatio(const QPoint &targetPosition);
    void saveMarginRatioMap();
    void loadMarginRatioMap();

    void moveVirtualKeyboard(const QPoint &targetPoint);

    constexpr static float defaultBottomMarginRatio = 0.05f;

    float leftMarginRatio_ = 0.0f;
    float topMarginRatio_ = 0.0f;

    static const QString marginRatioMapKey;
    static const QString leftMarginRatioKey;
    static const QString topMarginRatioKey;
};

#endif // FLOATGEOMETRYMANAGER_H
