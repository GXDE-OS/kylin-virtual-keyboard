import QtQuick 2.0
import QtQuick.Controls 2.0

Rectangle {
    width: virtualKeyboard.width
    height: virtualKeyboard.toolAndCandidateHeight
    color: virtualKeyboard.virtualKeyboardColor
    visible: !virtualKeyboard.isToolbarVisible

    ListView {
        x: virtualKeyboard.candidateListX
        width: virtualKeyboard.candidateListWidth
        height: parent.height
        visible: true
        boundsBehavior: Flickable.StopAtBounds
        orientation: ListView.Horizontal
        model: virtualKeyboard.candidateList
        spacing: 20
        delegate: Text {
            id: candidate
            text: modelData
            font.pointSize: virtualKeyboard.candidateFontSize
            anchors.verticalCenter: parent.verticalCenter
            MouseArea {
                anchors.fill: parent
                onClicked: { 
                    qmlCandidateClicked(index)
                    console.debug("点击了",index,modelData)
                }
            }
        }
    }

}

