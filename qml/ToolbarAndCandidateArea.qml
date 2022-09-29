import QtQuick 2.0

Rectangle {
    width: virtualKeyboard.width
    height: virtualKeyboard.toolAndCandidateHeight
    anchors.top: preedit.bottom
    color: virtualKeyboard.virtualKeyboardColor
    Toolbar{}
    CandidateList{}
}
