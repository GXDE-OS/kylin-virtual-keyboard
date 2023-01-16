import QtQuick 2.0

Rectangle {
    width: parent.width
    height: parent.height
    visible: virtualKeyboard.isToolbarVisible
    color: virtualKeyboard.virtualKeyboardColor

    HideButton{id: hideButton}
    PlacementModeButton{
        alignmentRight: hideButton
    }
}
