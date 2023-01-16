import QtQuick 2.0

Rectangle {
    id: dragBar
    width: virtualKeyboard.width
    height: virtualKeyboard.dragBarHeight
    color: virtualKeyboard.virtualKeyboardColor
    radius: virtualKeyboard.virtualKeyboardFloatPlacementRadius

    property int startX: 0;
    property int startY: 0;

    MouseArea {
        anchors.fill: parent
        onPressed: {
            startX = mouseX
            startY = mouseY
        }
        onPositionChanged: {
            qmlMoveByOffset(mouseX - startX, mouseY - startY)
        }
    }

    Rectangle {
        id: dragBarIndicator
        anchors.centerIn: parent
        width: virtualKeyboard.dragBarIndicatorWidth
        height: virtualKeyboard.dragBarIndicatorHeight
        color: virtualKeyboard.dragBarIndicatorColor
        radius: virtualKeyboard.dragBarIndicatorRadius
    }

    state: virtualKeyboard.placementMode
    states: [
        State {
            name: "EXPANSION"
            PropertyChanges {
                target: dragBar
                visible: false
            }
        },
        State {
            name: "FLOAT"
            PropertyChanges {
                target: dragBar
                visible: true
            }
        }
    ]
}
