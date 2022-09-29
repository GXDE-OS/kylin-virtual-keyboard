import QtQuick 2.0

SymbolKey {
    height: virtualKeyboard.firstRowKeyHeight

    property string fnValue

    state: virtualKeyboard.fnSymbolState

    states: [
        State {
            name: "FN"
            PropertyChanges {
                target: keyLabel
                text: fnValue
            }
            PropertyChanges {
                target: shiftLabel
                text: ""
            }
        }
    ]
}
