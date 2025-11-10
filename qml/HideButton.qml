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
import QtQuick.Controls 2.0

Rectangle {
    height: virtualKeyboard.toolbarSize
    width: virtualKeyboard.toolbarSize
    anchors.verticalCenter: parent.verticalCenter
    anchors.right: parent.right
    anchors.rightMargin: virtualKeyboard.cardinalNumber * 3.5
    color: virtualKeyboard.virtualKeyboardColor
    radius: virtualKeyboard.toolbarRadius

    Image {
        id: hideButtonImg
        anchors.centerIn: parent
        sourceSize: Qt.size(parent.width * 0.5, parent.width * 0.5)
        source: "qrc:/img/close.svg"
    }

    ToolTip {
        id:hideButtonToolTip
        text: qsTr("close")
        property var backOpacity: 1
    }

    MouseArea {
        anchors.fill: parent
        hoverEnabled: true

        onPressed: {
            color = virtualKeyboard.hideButtonPressedColor
        }

        onReleased: {
            virtualKeyboard.hideVirtualKeyboard()
            color = virtualKeyboard.hideButtonHoverColor
        }

        onEntered: {
            hideButtonToolTip.visible = true
            color = virtualKeyboard.hideButtonHoverColor
            hideButtonImg.source = "qrc:/img/close_hovered.svg"
        }

        onExited: {
            hideButtonToolTip.visible = false
            color = virtualKeyboard.virtualKeyboardColor
            hideButtonImg.source = "qrc:/img/close.svg"
        }
    }
}
