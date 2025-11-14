/*
 * Copyright (c) KylinSoft Co., Ltd. 2025.All rights reserved.
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

#include "waylandworkspaceadjuster.h"

#include <QMetaType>
#include <QVariant>
#include <QWindow>

#include "../log.h"
#include "ukuiwaylandhelper/ukuiwaylandproperties.h"

WaylandWlcomWorkspaceAdjuster::WaylandWlcomWorkspaceAdjuster() {
    KVKBD_INFO("wayland wlcom workspace adjuster.");
}

void WaylandWlcomWorkspaceAdjuster::raiseInputArea(QWindow *window, 
                                                    const QRect &rect) {
    KVKBD_INFO("raise inputarea.");
    if (window == nullptr) {
        KVKBD_WARN("window is null, skip raiseInputArea.");
        return;
    }
    surfaceWindow_ = window;
    setSurfaceWindowProperty(rect, 1);
}

void WaylandWlcomWorkspaceAdjuster::fallInputArea() {
    KVKBD_INFO("fall inputarea.");
    if (surfaceWindow_ == nullptr) {
        KVKBD_WARN("surfaceWindow_ is null, skip fallInputArea.");
        return;
    }
    QRect fallRect(0, 0, 0, 0);
    setSurfaceWindowProperty(fallRect, 0);
}

void WaylandWlcomWorkspaceAdjuster::setSurfaceWindowProperty(
    const QRect &rect, const int32_t &enabled) {
    UkuiWaylandProperty::SurfaceProperty property;
    property.height = rect.height();
    property.area = 1;
    property.zone = rect.height();
    property.enabled = enabled;
    surfaceWindow_->setProperty(UkuiWaylandProperty::SURFACE_ANCHOR,
                                QVariant::fromValue(property.toVector()));
}
