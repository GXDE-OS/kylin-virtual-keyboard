import QtQuick 2.0
import QtQuick.Controls 2.12
import QtQml 2.1
import "../js/utils.js" as Utils

SwitchKey {
    id: changeIm
    label: virtualKeyboard.currentIM.split("|")[2]
    
    Binding {
        target: changeIm
        property: "switchKeyState"
        value: virtualKeyboard.changeIMState
    }
    
    keyMouseArea.onReleased: {
        if(virtualKeyboard.changeIMState == "NORMAL"){
            virtualKeyboard.changeIMState = "OPEN"
            imList.open()
        }else{
            imList.close()
        }
    }
    
    keyMouseArea.onPressed: {
        virtualKeyboard.qmlRequestCurrentIMList()
        imList.currentIndex =
                virtualKeyboard.currentIMList.indexOf(virtualKeyboard.currentIM)
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
        width: imList.width
        height: imList.height
        Menu {
            id: imList
            
            onHeightChanged: {
                // 切换输入法列表默认显示四个条目
                if (imList.count > 4) {
                    height = imList.itemAt(0).height * 4
                }
            }
            
            modal: true
            Overlay.modal:Rectangle {
                color: "transparent"
            }
            background: Rectangle{
                color: "white"
            }
            
            Instantiator {
                model: virtualKeyboard.currentIMList
                delegate: MenuItem {
                    background: Rectangle {
                        color: highlighted? virtualKeyboard.currentIMColor: "white"
                    }
                    property string uniqueName: modelData.split("|")[0]
                    property string localName: modelData.split("|")[1]
                    property string label: modelData.split("|")[2]
                    font.pointSize: virtualKeyboard.imFontSize
                    text: label.padEnd(3, " ") + localName
                    MouseArea{
                        anchors.fill: parent
                        onReleased: {
                            virtualKeyboard.qmlSetCurrentIM(uniqueName)
                            imList.close()
                        }
                    }
                }
                
                onObjectAdded: imList.insertItem(index, object)
                onObjectRemoved: imList.removeItem(object)
            }
            
            onClosed: {
                virtualKeyboard.changeIMState = "NORMAL"
            }
        }
    }
    
}
