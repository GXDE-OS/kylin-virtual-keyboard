import QtQuick 2.0

Rectangle {
    height: virtualKeyboard.toolbarSize
    width: virtualKeyboard.toolbarSize

    property var alignmentRight

    anchors.verticalCenter: parent.verticalCenter
    anchors.right: alignmentRight.left
    anchors.rightMargin: virtualKeyboard.cardinalNumber * 3.5
    color: virtualKeyboard.virtualKeyboardColor
    Image {
        id: placementModelButtonImg
        sourceSize: Qt.size(parent.width, parent.width)
        source: "qrc:/img/upfloat.svg"
        anchors.centerIn: parent
    }

    MouseArea {
        anchors.fill: parent
        onClicked: qmlPlacementModeButtonClicked()
    }

    state: virtualKeyboard.placementMode
    states: [
        State {
            name: "EXPANSION"
            PropertyChanges {
                target: placementModelButtonImg
                source: "qrc:/img/upfloat.svg"
            }
        },
        State {
            name: "FLOAT"
            PropertyChanges {
                target: placementModelButtonImg
                source: "qrc:/img/downfloat.svg"
            }
        }
    ]
}
