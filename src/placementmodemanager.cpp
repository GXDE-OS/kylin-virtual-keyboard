#include "placementmodemanager.h"

#include "virtualkeyboardsettings.h"

// static
const QString PlacementModeManager::placementModeGroup = "placementMode";
// static
const QString PlacementModeManager::placementModeKey = "placementMode";

PlacementModeManager::PlacementModeManager(QObject *parent) : QObject(parent) {
    loadPlacementMode();
}

PlacementModeManager::~PlacementModeManager() { savePlacementMode(); }

void PlacementModeManager::updatePlacementMode() {
    if (placementMode_ == PlacementMode::Expansion) {
        enterExpansionMode();
    } else {
        enterFloatMode();
    }
}

void PlacementModeManager::flipPlacementMode() {
    if (placementMode_ == PlacementMode::Expansion) {
        enterFloatMode();
    } else {
        enterExpansionMode();
    }
}

void PlacementModeManager::setPlacementMode(PlacementMode placementMode) {
    placementMode_ = placementMode;
    savePlacementMode();
}

void PlacementModeManager::enterExpansionMode() {
    setPlacementMode(PlacementMode::Expansion);
    emit expansionModeEntered();
}

void PlacementModeManager::enterFloatMode() {
    setPlacementMode(PlacementMode::Float);
    emit floatModeEntered();
}

void PlacementModeManager::savePlacementMode() {
    VirtualKeyboardSettings::getInstance().setValue(
        placementModeGroup, placementModeKey, placementMode_);
}

void PlacementModeManager::loadPlacementMode() {
    placementMode_ = VirtualKeyboardSettings::getInstance()
                         .getValue(placementModeGroup, placementModeKey,
                                   PlacementModeManager::Expansion)
                         .value<PlacementModeManager::PlacementMode>();
}
