import QtQuick 2.0
import "../js/utils.js" as Utils

SwitchKey {
    signal temporarySwitchKeyReleased(string temporarySwitchKeyState)
    signal temporarySwitchKeyClicked()

    keyMouseArea.onReleased: {
        if(switchKeyState == "NORMAL") {
            switchKeyState = "OPEN"
        } else {
            switchKeyState = "NORMAL"
            var modifierKeyState = Utils.getModifierKeyStates()
            virtualKeyboard.processKeyEvent(label, Utils.getKeyCode(label), modifierKeyState - Utils.getKeySym(label), false, Date())
            virtualKeyboard.processKeyEvent(label, Utils.getKeyCode(label), modifierKeyState, true, Date())
            temporarySwitchKeyClicked()
        }
        temporarySwitchKeyReleased(switchKeyState)
    }

}
