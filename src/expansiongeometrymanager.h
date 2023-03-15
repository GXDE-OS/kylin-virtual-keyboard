#ifndef EXPANSIONGEOMETRYMANAGER_H
#define EXPANSIONGEOMETRYMANAGER_H

#include "geometrymanager.h"

class ExpansionGeometryManager : public GeometryManager {
public:
    ExpansionGeometryManager();
    ~ExpansionGeometryManager() override = default;

private:
    float getViewWidthRatio() const override;
    float getViewHeightRatio() const override;
    QPoint calculateViewPosition() override;
};

#endif // EXPANSIONGEOMETRYMANAGER_H
