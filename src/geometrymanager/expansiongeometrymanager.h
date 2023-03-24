#ifndef EXPANSIONGEOMETRYMANAGER_H
#define EXPANSIONGEOMETRYMANAGER_H

#include "geometrymanager.h"

class ExpansionGeometryManager : public GeometryManager {
public:
    ExpansionGeometryManager();
    ~ExpansionGeometryManager() override = default;

private:
    int getUnitWidth() const override;
    float getViewWidthRatio() const override;
    int calculateViewHeight() const override;
    QPoint calculateViewPosition() override;

private:
    constexpr static float viewHeightRatio_ = 512.0 / 1620.0;
};

#endif // EXPANSIONGEOMETRYMANAGER_H
