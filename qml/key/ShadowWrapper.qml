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
    onSourceChanged: {
        if (effectLoader.item)
            effectLoader.item.source = wrapper.source;

    }
    onColorChanged: {
        if (effectLoader.item)
            effectLoader.item.color = wrapper.color;

    }
    onHorizontalOffsetChanged: {
        if (effectLoader.item)
            effectLoader.item.horizontalOffset = wrapper.horizontalOffset;

    }
    onVerticalOffsetChanged: {
        if (effectLoader.item)
            effectLoader.item.verticalOffset = wrapper.verticalOffset;

    }
    onRadiusChanged: {
        if (effectLoader.item)
            effectLoader.item.radius = wrapper.radius;

    }
    onSamplesChanged: {
        if (effectLoader.item)
            effectLoader.item.samples = wrapper.samples;

    }
    onZChanged: {
        if (effectLoader.item)
            effectLoader.item.z = wrapper.z;

    }

    Loader {
        id: effectLoader

        anchors.fill: parent
        source: isQt6 ? "GraphicalEffectsQt6.qml" : "GraphicalEffectsQt5.qml"
        onLoaded: {
            if (item) {
                item.source = wrapper.source;
                item.color = wrapper.color;
                item.horizontalOffset = wrapper.horizontalOffset;
                item.verticalOffset = wrapper.verticalOffset;
                item.radius = wrapper.radius;
                item.samples = wrapper.samples;
                item.z = wrapper.z;
            }
        }
    }

}
