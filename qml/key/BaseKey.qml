import QtQuick 2.0
import QtQuick.Controls 2.0
import QtGraphicalEffects 1.0

ToolButton {
    width: virtualKeyboard.keyWidth
    height: virtualKeyboard.keyHeight
    property string label: ""
    property real fontSize: virtualKeyboard.fontSize
    property alias keyLabel: keyLabel_
    property alias keyBackground: keyBackground_
    /*label和img对应父控件的水平对齐方式 取值为AlignHCenter:居中对齐,AlignRight:向右对齐,AlignLeft:向左对齐*/
    property int alignment: Text.AlignHCenter

    background: Rectangle {
        id: keyBackground_
        radius: virtualKeyboard.keyRadius
    }

    DropShadow {
        anchors.fill: parent
        horizontalOffset: 2
        verticalOffset: 3
        radius: virtualKeyboard.keyRadius
        color: "#999999"
        source: keyBackground
    }


    Label {
        id: keyLabel_
        text: label
        font.pointSize: fontSize
        font.weight: Font.Light
        anchors.fill: parent
        padding: alignment == Text.AlignHCenter ? undefined : virtualKeyboard.keyLableAlignment
        horizontalAlignment: alignment
        verticalAlignment: Text.AlignVCenter
        visible: true
    }
}

