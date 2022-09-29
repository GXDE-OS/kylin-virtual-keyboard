import QtQuick 2.0

Rectangle {
    width: virtualKeyboard.width
    height: virtualKeyboard.keyboardLayoutHeight
    anchors.top: toolbarAndCandidate.bottom
    visible: virtualKeyboard.isToolAreaVisible

}
