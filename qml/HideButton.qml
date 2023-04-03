import QtQuick 2.0
import QtQuick.Controls 2.0

Rectangle {
    height: virtualKeyboard.toolbarSize
    width: virtualKeyboard.toolbarSize
    anchors.verticalCenter: parent.verticalCenter
    anchors.right: parent.right
    anchors.rightMargin: virtualKeyboard.cardinalNumber * 3.5
    color: virtualKeyboard.virtualKeyboardColor
    Image {
        id: hideButtonImg
        anchors.centerIn: parent
        sourceSize: Qt.size(parent.width, parent.width)
        source: "qrc:/img/close.svg"
    }

    MouseArea {
        anchors.fill: parent

        onPressed: {
            hideButtonImg.source = "qrc:/img/close_pressed.svg"
        }

        onReleased: {
            virtualKeyboard.hideVirtualKeyboard()
        }
    }


}
