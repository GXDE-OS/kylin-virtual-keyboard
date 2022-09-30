import QtQuick 2.0

ActionKey {
    width: virtualKeyboard.keyWidth*1.5+virtualKeyboard.keySpacing
    height: keyHeight * 3/4
    alignment: Text.AlignRight
    label: "Backspace"
    actionKeyImgPath: "qrc:/img/backspace.png"
}
