import QtQuick 2.0
import QtQuick.Controls 2.12
import "../js/utils.js" as Utils

SwitchKey {
    id: changeIm
    label: "中"

    Binding {
        target: changeIm
        property: "switchKeyState"
        value: virtualKeyboard.changeIMState
    }

    keyMouseArea.onReleased: {
        if(virtualKeyboard.changeIMState == "NORMAL"){
            virtualKeyboard.changeIMState = "OPEN"
            virtualKeyboard.qmlRequestCurrentIMList()
            imList.open()
        }else{
            imList.close()
        }
    }

    state: virtualKeyboard.changeIMState
    states: [
        State {
            name: "NORMAL"
            PropertyChanges {
                target: keyBackground
                state: "NORMAL"
            }
            PropertyChanges {
                target: virtualKeyboard
                isCurrentIMListVisible : false
            }
        },
        State {
            name: "OPEN"
            PropertyChanges {
                target: keyBackground
                state: "OPEN"
            }
            PropertyChanges {
                target: virtualKeyboard
                isCurrentIMListVisible : true
            }
        }
    ]

    Rectangle {
        id: imListRectangle
        anchors.bottom: parent.top
        anchors.horizontalCenter: parent.horizontalCenter
        visible: virtualKeyboard.isCurrentIMListVisible
        width: virtualKeyboard.imWidth
        height: Math.min(currentIMListView.count, 4) * virtualKeyboard.imHeight
        Menu {
            id: imList
            width: parent.width
            height: parent.height
            modal: true
            Overlay.modal:Rectangle {
                color: "transparent"
            }

            ListView {
                id: currentIMListView
                width: parent.width
                height: imListRectangle.height
                model: virtualKeyboard.currentIMList
                boundsBehavior: Flickable.StopAtBounds
                highlight: Rectangle{color: virtualKeyboard.currentIMColor}
                delegate: Text {
                    height: virtualKeyboard.imHeight
                    width: virtualKeyboard.imWidth
                    font.pointSize: virtualKeyboard.imFontSize
                    text: modelData
                    anchors.left: parent.left
                    anchors.leftMargin: virtualKeyboard.imLeftMargin
                    MouseArea{
                       anchors.fill: parent
                       onReleased: {
                           virtualKeyboard.qmlSetCurrentIM(modelData)
                           currentIMListView.currentIndex = index
                           imList.close()
                       }
                    }
                }
            }

            onClosed: {
                virtualKeyboard.changeIMState = "NORMAL"
            }
        }
    }
}
