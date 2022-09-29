import QtQuick 2.0
import "../js/utils.js" as Utils

SwitchKey {
    id: fn
    label: "Fn"

    switchKeyState: fn.state
    keyMouseArea.onReleased: {
        if(fn.state == "NORMAL") {
            fn.state = "OPEN"
        } else {
            fn.state = "NORMAL"
        }
    }

    state: "NORMAL"
    states: [
        State {
            name: "NORMAL"
            PropertyChanges {
                target: virtualKeyboard
                fnSymbolState : virtualKeyboard.shiftState == "NORMAL" ? "NORAML" : "SHIFT"
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
               fnSymbolState : "FN"
           }
            PropertyChanges {
                target: keyBackground
                state: "OPEN"
            }
        }
    ]

}
