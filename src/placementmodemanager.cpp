#include "placementmodemanager.h"

PlacementModeManager::PlacementModeManager(VirtualKeyboardManager* virtualKeyboardManager, QObject *parent)
    : QObject(parent), virtualKeyboardManager_(virtualKeyboardManager)
{
    
}

void PlacementModeManager::updatePlacementMode()
{
    if(placementMode_ == PlacementMode::Expansion) {
        enterExpansionMode();
    }
    else {
        enterFloatMode();
    }
}

void PlacementModeManager::flipPlacementMode() 
{
    if(placementMode_ == PlacementMode::Expansion) {
        enterFloatMode();
    }
    else {
        enterExpansionMode();
    }
}

void PlacementModeManager::enterExpansionMode()
{
    placementMode_ = PlacementMode::Expansion;
    savePlacementMode();
    emit expansionModeEntered();
}

void PlacementModeManager::enterFloatMode()
{
    placementMode_ = PlacementMode::Float;
    savePlacementMode();
    emit floatModeEntered();
}

void PlacementModeManager::savePlacementMode()
{
    
}

PlacementMode PlacementModeManager::loadPlacementMode()
{
    return PlacementMode::Float;
}
