#ifndef EXPANSIONGEOMETRYMANAGER_H
#define EXPANSIONGEOMETRYMANAGER_H

#include "geometrymanager.h"

class ExpansionGeometryManager : public GeometryManager {
public:
    explicit ExpansionGeometryManager(
        VirtualKeyboardManager *virtualKeyboardManager,
        QObject *parent = nullptr);
    ~ExpansionGeometryManager() = default;

private:
    QPoint calculateVirtualKeyboardPosition() override;
    QSize calculateVirtualKeyboardSize() override;

    float virtualKeyboardAspectRatio_ = 512.0 / 1620.0;
};

#endif // EXPANSIONGEOMETRYMANAGER_H
