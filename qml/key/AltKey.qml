import QtQuick 2.0
import "../js/utils.js" as Utils

TemporarySwitchKey {
    id: alt
    label: "Alt"

    Binding {
        target: alt
        property: "switchKeyState"
        value: virtualKeyboard.altState
    }

    Component.onCompleted: {
        temporarySwitchKeyReleased.connect(updateAltState)
        temporarySwitchKeyClicked.connect(virtualKeyboard.altClicked)
    }

    function updateAltState(altState) {
        virtualKeyboard.altState = altState
    }

    state: virtualKeyboard.altState
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
