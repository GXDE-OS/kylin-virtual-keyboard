#ifndef EXPANSIONGEOMETRYMANAGER_H
#define EXPANSIONGEOMETRYMANAGER_H

#include "geometrymanager.h"

class ExpansionGeometryManager : public GeometryManager {
public:
    explicit ExpansionGeometryManager(QObject *parent = nullptr);
    ~ExpansionGeometryManager() override = default;

private:
    float getVirtualKeyboardWidthRatio() const override;
    float getVirtualKeyboardHeightRatio() const override;
    QPoint calculateVirtualKeyboardPosition() override;
};

#endif // EXPANSIONGEOMETRYMANAGER_H
