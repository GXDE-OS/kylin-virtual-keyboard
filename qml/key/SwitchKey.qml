import QtQuick 2.0
import "../js/utils.js" as Utils

BaseKey {

    property color normalColor: virtualKeyboard.switchKeyNormalColor
    property color pressedColor: virtualKeyboard.switchKeyPressedColor
    property color openColor: virtualKeyboard.switchKeyOpenColor
    property color openPressedColor: virtualKeyboard.switchKeyOpenPressedColor
    property color hoverNormalColor: virtualKeyboard.switchKeyHoverNormalColor
    property color hoverOpenColor: virtualKeyboard.switchKeyHoverOpenColor
    property alias keyMouseArea: keyMouseArea_
    property string switchKeyState

    fontSize: virtualKeyboard.switchKeyFontSize

    Component.onCompleted: {
        charKeyClicked.connect(switchKeyNormal)
    }
    function switchKeyNormal() {
        virtualKeyboard.shiftState = "NORMAL"
        virtualKeyboard.ctrlState = "NORMAL"
        virtualKeyboard.altState = "NORMAL"
        virtualKeyboard.winState = "NORMAL"
    }


    keyBackground.state: "NORMAL"
    keyBackground.states: [
        State {
            name: "NORMAL"
            PropertyChanges {
                target: keyBackground
                color: normalColor
            }
            PropertyChanges{
                target: dropShadow
                verticalOffset: virtualKeyboard.dropShadowVerticalOffset
                color: virtualKeyboard.switchKeyNormalDropShadowColor
            }
        },
        State {
            name: "PRESSED"
            PropertyChanges {
                target: keyBackground
                color: pressedColor
            }
            PropertyChanges{
                target: dropShadow
                verticalOffset: -virtualKeyboard.dropShadowVerticalOffset
                color: virtualKeyboard.switchKeyNormalDropShadowColor
            }
        },
        State {
            name: "OPEN"
            PropertyChanges {
                target: keyBackground
                color: openColor
            }
            PropertyChanges{
                target: dropShadow
                verticalOffset: -virtualKeyboard.dropShadowVerticalOffset
                color: virtualKeyboard.switchKeyOpenDropShadowColor
            }
        },
        State {
            name: "OPEN_PRESSED"
            PropertyChanges {
                target: keyBackground
                color: openPressedColor
            }
            PropertyChanges{
                target: dropShadow
                verticalOffset: -virtualKeyboard.dropShadowVerticalOffset
                color: virtualKeyboard.switchKeyOpenPressedDropShadowColor
            }
        },
        State {
            name: "HOVER_NORMAL"
            PropertyChanges {
                target: keyBackground
                color: hoverNormalColor
            }
        },
        State {
            name: "HOVER_OPEN"
            PropertyChanges {
                target: keyBackground
                color: hoverOpenColor
            }
            PropertyChanges{
                target: dropShadow
                verticalOffset: -virtualKeyboard.dropShadowVerticalOffset
                color: virtualKeyboard.switchKeyOpenDropShadowColor
            }
        }
    ]

    MouseArea {
        id: keyMouseArea_
        anchors.fill: parent
        hoverEnabled: true

        onPressed: {
            if(switchKeyState == "NORMAL") {
                keyBackground.state = "PRESSED"
            } else {
                keyBackground.state = "OPEN_PRESSED"
            }
        }

        onEntered: {
            if(switchKeyState == "NORMAL") {
                keyBackground.state = "HOVER_NORMAL"
            } else {
                keyBackground.state = "HOVER_OPEN"
            }
        }

        onExited: {
            if(switchKeyState == "NORMAL") {
                keyBackground.state = "NORMAL"
            } else {
                keyBackground.state = "OPEN"
            }
        }
    }
}
