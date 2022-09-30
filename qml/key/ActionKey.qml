import QtQuick 2.0

CharKey {
    normalColor: virtualKeyboard.actionKeyNormalColor
    pressedColor: virtualKeyboard.actionKeyPressedColor
    hoverColor: virtualKeyboard.actionKeyHoverColor

    fontSize: virtualKeyboard.actionKeyFontSize

    property string actionKeyImgPath
    keyLabel.visible: actionKeyImgPath == ""

    Loader{
        width: virtualKeyboard.toolbarSize
        height: virtualKeyboard.toolbarSize
        anchors.verticalCenter: parent.verticalCenter
        sourceComponent: actionKeyImgPath == "" ? undefined : actionKeyComponent
        anchors{
            horizontalCenter: alignment == Text.AlignHCenter ? parent.horizontalCenter : undefined
            right: alignment == Text.AlignRight ? parent.right : undefined
            rightMargin: alignment == Text.AlignRight ? virtualKeyboard.keyIconAlignment : undefined
            left: alignment == Text.AlignLeft ? parent.left : undefined
            leftMargin: alignment == Text.AlignLeft ? virtualKeyboard.keyIconAlignment : undefined
        }
    }

    Component{
        id: actionKeyComponent
        Image {
            source: actionKeyImgPath
        }
    }
}
