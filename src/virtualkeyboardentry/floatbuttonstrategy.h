#ifndef FLOATBUTTONSTRATEGY_H
#define FLOATBUTTONSTRATEGY_H

#include "geometrymanager/floatgeometrymanager.h"
#include "screenmanager.h"

class FloatButtonStrategy : public FloatGeometryManager::Strategy {
public:
    FloatButtonStrategy() = default;
    ~FloatButtonStrategy() override = default;

    int getDefaultRightMargin() const override {
        return getUnitWidth() * defaultRightMarginRatio_;
    }

    int getDefaultBottomMargin() const override {
        return getUnitHeight() * defaultBottomMarginRatio_;
    }

private:
    int getUnitWidth() const override {
        const auto viewPortSize = ScreenManager::getPrimaryScreenSize();

        return std::max(viewPortSize.width(), viewPortSize.height());
    }

    int getUnitHeight() const override {
        const auto viewPortSize = ScreenManager::getPrimaryScreenSize();

        return std::max(viewPortSize.width(), viewPortSize.height());
    }

    float getViewWidthRatio() const override { return 56.0 / 1620.0; }

    float getViewHeightRatio() const override { return 56.0 / 1620.0; }

private:
    static constexpr float defaultBottomMarginRatio_ = 0.03f;
    static constexpr float defaultRightMarginRatio_ = 0.02f;
};

#endif // FLOATBUTTONSTRATEGY_H
