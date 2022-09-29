import QtQuick 2.0
import "../js/utils.js" as Utils

SwitchKey {
    label: "Caps"

    switchKeyState: virtualKeyboard.capslockState
    keyMouseArea.onReleased: {
        if(virtualKeyboard.capslockState == "NORMAL") {
            virtualKeyboard.capslockState = "OPEN"
        } else {
            virtualKeyboard.capslockState = "NORMAL"
        }
        var modifierKeyState = Utils.getModifierKeyStates()
        qmlKeyEvent(label, Utils.getKeyCode(label), modifierKeyState, false, Date())
        qmlKeyEvent(label, Utils.getKeyCode(label), modifierKeyState, true, Date())
    }

    state: virtualKeyboard.capslockState
    states: [
        State {
            name: "NORMAL"
            PropertyChanges {
                target: virtualKeyboard
                letterState : virtualKeyboard.shiftState == "NORMAL" ? "NORMAL" : "SHIFT"
            }
            PropertyChanges {
                target: keyBackground
                state: "NORMAL"
            }
        },
        State {
            name: "OPEN"
            PropertyChanges {
                target: virtualKeyboard
                letterState : virtualKeyboard.shiftState == "NORMAL" ? "SHIFT" : "NORMAL"
            }
            PropertyChanges {
                target: keyBackground
                state: "OPEN"
            }
        }
    ]

}
