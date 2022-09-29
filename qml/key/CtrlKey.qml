import QtQuick 2.0
import "../js/utils.js" as Utils

TemporarySwitchKey {
    id: ctrl
    label: "Ctrl"

    Binding {
        target: ctrl
        property: "switchKeyState"
        value: virtualKeyboard.ctrlState
    }

    Component.onCompleted: {
        temporarySwitchKeyReleased.connect(updateCtrlState)
        temporarySwitchKeyClicked.connect(virtualKeyboard.ctrlClicked)
    }

    function updateCtrlState(ctrlState) {
        virtualKeyboard.ctrlState = ctrlState
    }

    state: virtualKeyboard.ctrlState
    states: [
        State {
            name: "NORMAL"
            PropertyChanges {
                target: keyBackground
                state: "NORMAL"
            }
        },
        State {
            name: "OPEN"
            PropertyChanges {
                target: keyBackground
                state: "OPEN"
            }
        }
    ]

}
