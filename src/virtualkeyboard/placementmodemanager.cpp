/*
 * Copyright 2022 KylinSoft Co., Ltd.
 *
 * This program is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later
 * version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program. If not, see <https://www.gnu.org/licenses/>.
 */

#include "placementmodemanager.h"

#include "localsettings/localsettings.h"

// static
const QString PlacementModeManager::placementModeGroup = "placementMode";
// static
const QString PlacementModeManager::placementModeKey = "placementMode";

PlacementModeManager::PlacementModeManager(LocalSettings &viewSettings)
    : QObject(), viewSettings_(viewSettings) {
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
    viewSettings_.setValue(placementModeGroup, placementModeKey,
                           placementMode_);
}

void PlacementModeManager::loadPlacementMode() {
    placementMode_ = viewSettings_
                         .getValue(placementModeGroup, placementModeKey,
                                   PlacementModeManager::Expansion)
                         .value<PlacementModeManager::PlacementMode>();
}
