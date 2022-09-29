import QtQuick 2.0

CharKey {

    state: virtualKeyboard.letterState

    states: [
        State {
            name: "NORMAL"
            PropertyChanges {
                target: keyLabel
                text: label
            }
        },

        State {
            name: "SHIFT"
            PropertyChanges {
                target: keyLabel
                text: shiftedText
            }
        }
    ]
}
