#ifndef VIRTUALKEYBOARDSTRATEGY_H
#define VIRTUALKEYBOARDSTRATEGY_H

#include "floatgeometrymanager.h"

#include "screenmanager.h"

class VirtualKeyboardStrategy : public FloatGeometryManager::Strategy {
public:
    VirtualKeyboardStrategy() = default;
    ~VirtualKeyboardStrategy() override = default;

    int getUnitWidth() const override {
        return ScreenManager::getPrimaryScreenSize().width();
    }

    float getViewWidthRatio() const override { return 1458.0 / 1620.0; }
    float getViewHeightRatio() const override { return 548.0 / 1620.0; }

    float getDefaultBottomMarginRatio() const override { return 0.05f; }
};

#endif // VIRTUALKEYBOARDSTRATEGY_H
