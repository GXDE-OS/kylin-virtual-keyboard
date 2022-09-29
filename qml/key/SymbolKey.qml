import QtQuick 2.0
import QtQuick.Controls 2.0

CharKey {

    property alias shiftLabel: shiftLabel_

    state: virtualKeyboard.symbolState

    states: [
        State {
            name: "NORMAL"
            PropertyChanges {
                target: keyLabel
                text: label
            }
            PropertyChanges {
                target:shiftLabel
                text: shiftedText
            }
        },

        State {
            name: "SHIFT"
            PropertyChanges {
                target: keyLabel
                text: shiftedText
            }
            PropertyChanges {
                target: shiftLabel
                text: ""
            }
        }
    ]

    Label {
        id: shiftLabel_
        text: shiftedText
        font.pointSize: virtualKeyboard.shiftFontSize
        font.weight: Font.Light
        anchors{
            left: parent.left
            leftMargin: virtualKeyboard.shiftLeftMargin
            top: parent.top
            topMargin: virtualKeyboard.topLeftMargin
        }
        visible: true
    }
}
