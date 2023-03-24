#ifndef VIRTUALKEYBOARDSTRATEGY_H
#define VIRTUALKEYBOARDSTRATEGY_H

#include "geometrymanager/floatgeometrymanager.h"

#include "screenmanager.h"

class VirtualKeyboardStrategy : public FloatGeometryManager::Strategy {
public:
    VirtualKeyboardStrategy() = default;
    ~VirtualKeyboardStrategy() override = default;

    int getDefaultRightMargin() const override {
        const int screenWidth = ScreenManager::getPrimaryScreenSize().width();

        return (screenWidth - getViewWidth()) / 2;
    }

    int getDefaultBottomMargin() const override {
        const int screenHeight = ScreenManager::getPrimaryScreenSize().height();

        return screenHeight * defaultBottomMarginRatio_;
    }

private:
    int getUnitWidth() const override {
        return ScreenManager::getPrimaryScreenSize().width();
    }

    float getViewWidthRatio() const override { return 1458.0 / 1620.0; }

    int getUnitHeight() const override {
        const auto viewPortSize = ScreenManager::getPrimaryScreenSize();

        return std::max(viewPortSize.width(), viewPortSize.height());
    }

    float getViewHeightRatio() const override { return 548.0 / 1620.0; }

private:
    static constexpr float defaultBottomMarginRatio_ = 0.05f;
};

#endif // VIRTUALKEYBOARDSTRATEGY_H
