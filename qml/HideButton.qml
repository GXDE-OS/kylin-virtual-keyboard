import QtQuick 2.0
import QtQuick.Controls 2.0

Rectangle {
    height: parent.height * 3/5
    width: height
    anchors.verticalCenter: parent.verticalCenter
    anchors.right: parent.right
    anchors.rightMargin: 10
    color: virtualKeyboard.virtualKeyboardColor
    Image {
        anchors.fill: parent
        source: "qrc:/img/close.png"
    }

    MouseArea {
        anchors.fill: parent

        onReleased: {
            qmlHideVirtualKeyboard()
        }
    }


}
