import QtQuick 2.0
import QtQuick.Controls 2.0
import "../js/utils.js" as Utils

BaseKey {
    property string shiftedText
    property color normalColor: virtualKeyboard.charKeyNormalColor
    property color pressedColor: virtualKeyboard.charKeyPressedColor
    property color hoverColor: virtualKeyboard.charKeyHoverColor

    MouseArea {
        id: keyMouseArea
        anchors.fill: parent
        hoverEnabled: true

        onReleased: {
            keyBackground.state = "NORMAL"
            timer.stop()
            var modifierKeyState = Utils.getModifierKeyStates()
            var keycode = Utils.getKeyCode(keyLabel.text)
            virtualKeyboard.processKeyEvent(keyLabel.text, keycode, modifierKeyState, false, Date())
            charKeyClicked()
        }

        onPressed: {
            keyBackground.state = "PRESSED"
            timer.start()
        }

        onEntered: {
            keyBackground.state = "HOVER"
        }

        onExited: {
            keyBackground.state = "NORMAL"
        }

    }

    keyBackground.state: "NORMAL"
    keyBackground.states: [
        State {
            name: "NORMAL"
            PropertyChanges {
                target: keyBackground
                color: normalColor
            }
            PropertyChanges {
                target: dropShadow
                verticalOffset: virtualKeyboard.dropShadowVerticalOffset
            }
        },
        State {
            name: "PRESSED"
            PropertyChanges {
                target: keyBackground
                color: pressedColor
            }
            PropertyChanges {
                target: dropShadow
                verticalOffset: -virtualKeyboard.dropShadowVerticalOffset
            }
        },
        State {
            name: "HOVER"
            PropertyChanges {
                target: keyBackground
                color: hoverColor
            }
        }
    ]


    Timer {
        id:timer
        interval: virtualKeyboard.longPressInterval
        repeat: true
        onTriggered: {
            var modifierKeyState = Utils.getModifierKeyStates()
            virtualKeyboard.processKeyEvent(keyLabel.text, Utils.getKeyCode(keyLabel.text), modifierKeyState, false, Date())
        }
    }
}
