import QtQuick 2.0
import "../js/utils.js" as Utils

TemporarySwitchKey {
    id: win
    //暂时以Win显示，后续更换麒麟icon
    label: "Win"

    Binding {
        target: win
        property: "switchKeyState"
        value: virtualKeyboard.winState
    }

    Component.onCompleted: {
        temporarySwitchKeyReleased.connect(updateWinState)
        temporarySwitchKeyClicked.connect(virtualKeyboard.winClicked)
    }

    function updateWinState(winState) {
        virtualKeyboard.winState = winState
    }

    state: virtualKeyboard.winState
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
