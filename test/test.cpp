#include <QApplication>
#include <QQuickView>

#include <cassert>

#include "../src/screenmanager.h"
#include "../src/virtualkeyboardsettings/virtualkeyboardsettings.h"
#include "../src/animation/animationfactory.h"
#include "../src/animation/disabledanimator.h"
#include "../src/animation/enabledanimator.h"
#include "../src/animation/expansionanimationfactory.h"
#include "../src/animation/floatanimationfactory.h"

void testGetPrimaryScreenGeometry() {
    auto rect = ScreenManager::getPrimaryScreenGeometry();
    assert(!rect.isEmpty());
}

void testGetPrimaryScreenSize() {
    auto rect = ScreenManager::getPrimaryScreenSize();
    assert(!rect.isEmpty());
}

void testVirtualKeyboardSettings() {
    auto& settings = VirtualKeyboardSettings::getInstance();
    assert(settings.isAnimationEnabled());

    settings.updateFloatButtonAvailability(true);
    assert(settings.isFloatButtonEnabled());
    settings.updateFloatButtonAvailability(false);
    assert(!settings.isFloatButtonEnabled());

    auto scaleFactor = settings.calculateVirtualKeyboardScaleFactor();
    assert(scaleFactor > 0.1);
}

std::unique_ptr<AnimationFactory> createFloatModeAnimationFactory() {
    return std::unique_ptr<AnimationFactory>(
                new FloatAnimationFactory());
}

std::unique_ptr<AnimationFactory> createExpansionModeAnimationFactory() {
    return std::unique_ptr<AnimationFactory>(
        new ExpansionAnimationFactory());
}

void testAnimator(Animator* animator) {
    auto* view = new QQuickView();

    animator->playShowAnimation(view, {0, 0, 100, 100});
    animator->playHideAnimation(view, {0, 0, 0, 0});
    animator->playFlipAnimation(view, {0, 0, 200, 200});
}

void testVirtualKeyobardAnimation() {
    auto floatModeEnabledAnimator = std::unique_ptr<EnabledAnimator>(new EnabledAnimator(
        []() { return true; }, createFloatModeAnimationFactory()));
    testAnimator(floatModeEnabledAnimator.get());

    auto expansionModeEnabledAnimator = std::unique_ptr<EnabledAnimator>(new EnabledAnimator(
        []() { return false; }, createExpansionModeAnimationFactory()));
    testAnimator(expansionModeEnabledAnimator.get());

    auto disabledAnimator = std::unique_ptr<DisabledAnimator>(new DisabledAnimator());
    testAnimator(disabledAnimator.get());
}

int main(int argc, char* argv[]) {
     QApplication app(argc, argv);

    testGetPrimaryScreenGeometry();

    testGetPrimaryScreenSize();

    testVirtualKeyboardSettings();

    testVirtualKeyobardAnimation();

    return 0;
}
