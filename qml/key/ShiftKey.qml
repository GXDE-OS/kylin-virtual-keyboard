import QtQuick 2.0
import "../js/utils.js" as Utils

TemporarySwitchKey {
    id: shift
    label: "Shift"

    Binding {
        target: shift
        property: "switchKeyState"
        value: virtualKeyboard.shiftState
    }

    Component.onCompleted: {
        temporarySwitchKeyReleased.connect(updateShiftState)
        temporarySwitchKeyClicked.connect(virtualKeyboard.shiftClicked)
    }

    function updateShiftState(shiftState) {
        virtualKeyboard.shiftState = shiftState
    }

    state: virtualKeyboard.shiftState
    states: [
        State {
            name: "NORMAL"
            PropertyChanges {
                target: virtualKeyboard
                letterState : virtualKeyboard.capslockState == "NORMAL" ? "NORMAL" : "SHIFT"
                symbolState : "NORMAL"
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
                letterState : virtualKeyboard.capslockState == "NORMAL" ? "SHIFT" : "NORMAL"
                symbolState : "SHIFT"
            }
            PropertyChanges {
                target: keyBackground
                state: "OPEN"
            }
        }
    ]
}
