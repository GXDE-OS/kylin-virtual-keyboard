import QtQuick 2.0
import QtQuick.Controls 2.0

Rectangle {
    width: virtualKeyboard.width
    height: virtualKeyboard.preeditHeight
    color: virtualKeyboard.virtualKeyboardColor
    x: virtualKeyboard.preeditX
    anchors.top: dragBar.visible ? dragBar.bottom : parent.top

    Label {
        font.pointSize: virtualKeyboard.preeditTextFontSize
        anchors.verticalCenter: parent.verticalCenter
        text: virtualKeyboard.preeditText
        color: "black"
    }
}
