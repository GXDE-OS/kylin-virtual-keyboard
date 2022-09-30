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
        anchors.fill: parent
        source: "qrc:/img/close.png"
    }

    MouseArea {
        anchors.fill: parent

        onPressed: {
            hideButtonImg.source = "qrc:/img/close_pressed.png"
        }

        onReleased: {
            qmlHideVirtualKeyboard()
        }
    }


}
