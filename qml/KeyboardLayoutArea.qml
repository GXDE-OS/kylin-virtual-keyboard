import QtQuick 2.0
import QtQuick.Window 2.0

Rectangle {
    width: virtualKeyboard.width
    height: virtualKeyboard.keyboardLayoutHeight
    anchors.top: toolbarAndCandidate.bottom
    anchors.topMargin: keySpacing * 2
    color: virtualKeyboard.virtualKeyboardColor

    ClassicKeyboardLayout{}
}
