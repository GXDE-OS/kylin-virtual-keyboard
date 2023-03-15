#ifndef FLOATGEOMETRYMANAGER_H
#define FLOATGEOMETRYMANAGER_H

#include <memory>

#include "geometrymanager.h"
#include "localsettings.h"

class FloatGeometryManager : public GeometryManager {
    Q_OBJECT

public:
    class Strategy;

public:
    FloatGeometryManager(std::unique_ptr<Strategy> strategy,
                         LocalSettings &viewSettings);
    ~FloatGeometryManager() override;

public slots:
    void moveBy(int offsetX, int offsetY);
    void endDrag();

private:
    QPoint calculateViewPosition() override;
    int getUnitWidth() const override;
    float getViewWidthRatio() const override;
    int calculateViewHeight() const override;

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

    void moveView(const QPoint &targetPoint);

private:
    float leftMarginRatio_ = 0.0f;
    float topMarginRatio_ = 0.0f;

    std::unique_ptr<Strategy> strategy_;

    LocalSettings &viewSettings_;

    static const QString floatGeometryGroup;
    static const QString marginRatioMapKey;
    static const QString leftMarginRatioKey;
    static const QString topMarginRatioKey;
};

class FloatGeometryManager::Strategy {
public:
    virtual ~Strategy() = default;

    virtual int getUnitWidth() const = 0;
    virtual float getViewWidthRatio() const = 0;
    virtual float getViewHeightRatio() const = 0;

    virtual float getDefaultBottomMarginRatio() const = 0;

protected:
    Strategy() = default;
};

#endif // FLOATGEOMETRYMANAGER_H
