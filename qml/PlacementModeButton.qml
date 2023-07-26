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

import QtQuick 2.0

Rectangle {
    height: virtualKeyboard.toolbarSize
    width: virtualKeyboard.toolbarSize

    property var alignmentRight

    anchors.verticalCenter: parent.verticalCenter
    anchors.right: alignmentRight.left
    anchors.rightMargin: virtualKeyboard.cardinalNumber * 3.5
    color: virtualKeyboard.virtualKeyboardColor
    Image {
        id: placementModelButtonImg
        sourceSize: Qt.size(parent.width, parent.width)
        source: "qrc:/img/upfloat.svg"
        anchors.centerIn: parent
    }

    MouseArea {
        anchors.fill: parent
        onClicked: virtualKeyboard.flipPlacementMode()
    }

    state: virtualKeyboard.placementMode
    states: [
        State {
            name: "EXPANSION"
            PropertyChanges {
                target: placementModelButtonImg
                source: "qrc:/img/upfloat.svg"
            }
        },
        State {
            name: "FLOAT"
            PropertyChanges {
                target: placementModelButtonImg
                source: "qrc:/img/downfloat.svg"
            }
        }
    ]
}
