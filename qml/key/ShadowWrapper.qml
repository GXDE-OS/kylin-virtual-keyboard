/*
* Copyright 2025 KylinSoft Co., Ltd.
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

import QtQuick 2.0
import org.ukui.quick.items 1.0

Item {
    id: wrapper

    property var source
    property color color
    property real horizontalOffset: 0
    property real verticalOffset: 0
    property real radius: virtualKeyboard.keyRadius
    property int samples: 100
    property int qtVersionMajor: QT_VERSION_MAJOR
    property bool isQt6: qtVersionMajor >= 6

    anchors.fill: parent
    z: -1 // 阴影保持在最下层

    // 使用ShadowedRectangle来创建阴影，color设置为transparent，只显示阴影
    ShadowedRectangle {
        anchors.fill: parent
        radius: wrapper.radius
        color: "transparent"
        border.width: 0

        shadow {
            size: wrapper.radius
            color: wrapper.color
            xOffset: wrapper.horizontalOffset
            yOffset: wrapper.verticalOffset
        }

    }

}
