/*
* Copyright 2022 KylinSoft Co., Ltd.
*
* This program is free software: you can redistribute it and/or modify it under
* the terms of the GNU General Public License as published by the Free Software
* Foundation, either version 3 of the License, or (at your option) any later
* version.
*
* This program is distributed in the hope that it will be useful, but WITHOUT
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
* FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
* details.
*
* You should have received a copy of the GNU General Public License along with
* this program. If not, see <https://www.gnu.org/licenses/>.
*/

import QtQuick 2.0
import "key/"

Column {
    width: parent.width
    spacing: virtualKeyboard.keySpacing
    visible: virtualKeyboard.layout == "classic" ? true : false

    Row {
        anchors.horizontalCenter: parent.horizontalCenter
        spacing: virtualKeyboard.keySpacing
        ActionKey{ label: "Esc" ;height: virtualKeyboard.keyHeight * 3/4}
        FnSymbolKey{ label: "`"; shiftedText: "~"; fnValue:"~" }
        FnSymbolKey{ label: "1"; shiftedText: "!"; fnValue:"F1" }
        FnSymbolKey{ label: "2"; shiftedText: "@"; fnValue:"F2" }
        FnSymbolKey{ label: "3"; shiftedText: "#"; fnValue:"F3" }
        FnSymbolKey{ label: "4"; shiftedText: "$"; fnValue:"F4" }
        FnSymbolKey{ label: "5"; shiftedText: "%"; fnValue:"F5" }
        FnSymbolKey{ label: "6"; shiftedText: "^"; fnValue:"F6" }
        FnSymbolKey{ label: "7"; shiftedText: "&"; fnValue:"F7" }
        FnSymbolKey{ label: "8"; shiftedText: "*"; fnValue:"F8" }
        FnSymbolKey{ label: "9"; shiftedText: "("; fnValue:"F9" }
        FnSymbolKey{ label: "0"; shiftedText: ")"; fnValue:"F10" }
        FnSymbolKey{ label: "-"; shiftedText: "_"; fnValue:"F11" }
        FnSymbolKey{ label: "="; shiftedText: "+"; fnValue:"F12" }
        BackspaceKey{}
    }

    Row {
        anchors.horizontalCenter: parent.horizontalCenter
        spacing: virtualKeyboard.keySpacing
        ActionKey{ label: "Tab"; width: virtualKeyboard.keyWidth*1.5 + virtualKeyboard.keySpacing; alignment: Text.AlignLeft }
        LetterKey{ label: "q"; shiftedText: "Q" }
        LetterKey{ label: "w"; shiftedText: "W" }
        LetterKey{ label: "e"; shiftedText: "E" }
        LetterKey{ label: "r"; shiftedText: "R" }
        LetterKey{ label: "t"; shiftedText: "T" }
        LetterKey{ label: "y"; shiftedText: "Y" }
        LetterKey{ label: "u"; shiftedText: "U" }
        LetterKey{ label: "i"; shiftedText: "I" }
        LetterKey{ label: "o"; shiftedText: "O" }
        LetterKey{ label: "p"; shiftedText: "P" }
        SymbolKey{ label: "["; shiftedText: "{" }
        SymbolKey{ label: "]"; shiftedText: "}" }
        SymbolKey{ label: "\\"; shiftedText: "|" }
        ActionKey{ label: "Del" }
    }
    Row {
        anchors.horizontalCenter: parent.horizontalCenter
        spacing: virtualKeyboard.keySpacing

        CapslockKey{width: virtualKeyboard.keyWidth*2 + virtualKeyboard.keySpacing; alignment: Text.AlignLeft }
        LetterKey{ label: "a"; shiftedText: "A" }
        LetterKey{ label: "s"; shiftedText: "S" }
        LetterKey{ label: "d"; shiftedText: "D" }
        LetterKey{ label: "f"; shiftedText: "F" }
        LetterKey{ label: "g"; shiftedText: "G" }
        LetterKey{ label: "h"; shiftedText: "H" }
        LetterKey{ label: "j"; shiftedText: "J" }
        LetterKey{ label: "k"; shiftedText: "K" }
        LetterKey{ label: "l"; shiftedText: "L" }
        SymbolKey{ label: ";"; shiftedText: ":" }
        SymbolKey{ label: "'"; shiftedText: "\"" }
        EnterKey{}

    }
    Row {
        anchors.horizontalCenter: parent.horizontalCenter
        spacing: virtualKeyboard.keySpacing
        ShiftKey{width: virtualKeyboard.keyWidth*2.5 + virtualKeyboard.keySpacing*2; alignment: Text.AlignLeft }
        LetterKey{ label: "z" ; shiftedText: "Z" }
        LetterKey{ label: "x" ; shiftedText: "X" }
        LetterKey{ label: "c" ; shiftedText: "C" }
        LetterKey{ label: "v" ; shiftedText: "V" }
        LetterKey{ label: "b" ; shiftedText: "B" }
        LetterKey{ label: "n" ; shiftedText: "N" }
        LetterKey{ label: "m" ; shiftedText: "M" }
        SymbolKey{ label: ","; shiftedText: "<"  }
        SymbolKey{ label: "."; shiftedText: ">"  }
        SymbolKey{ label: "/"; shiftedText: "?"  }
        ActionKey{ label: "up"; actionKeyImgPath: "qrc:/img/up.svg" }
        ShiftKey{ width: virtualKeyboard.keyWidth*2 + virtualKeyboard.keySpacing; alignment: Text.AlignRight }
    }
    Row {
        anchors.horizontalCenter: parent.horizontalCenter
        spacing: virtualKeyboard.keySpacing
        CtrlKey{}
        FnKey{}
        //暂时删除win键
//        WinKey{}
        AltKey{}
        ChangeImKey{}
        SpaceKey{}
        AltKey{}
        ActionKey{ label: "left"; actionKeyImgPath: "qrc:/img/left.svg" }
        ActionKey{ label: "down"; actionKeyImgPath: "qrc:/img/down.svg" }
        ActionKey{ label: "right"; actionKeyImgPath: "qrc:/img/right.svg" }
        CtrlKey{}
    }
}

