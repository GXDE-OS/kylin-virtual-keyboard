function getKeySym(keyName){
    var keySymList = {
        "shift": 1,
        "caps": 2,
        "ctrl": 4,
        "alt": 8,
        "win": 64
    }
    return keySymList[keyName.toLowerCase()]
}

function getModifierKeyStates(){
    var result = 0
    result += shiftState=="NORMAL" ? 0 : getKeySym("shift")
    result += capslockState=="NORMAL" ? 0 : getKeySym("caps")
    result += ctrlState=="NORMAL" ? 0 : getKeySym("ctrl")
    result += altState=="NORMAL"? 0 : getKeySym("alt")
    result += winState=="NORMAL"? 0 : getKeySym("win")
    return result
}

function getKeyCode(keyName){
    var keyCodeList = {
        "esc": 0xff1b,
        "tab": 0xff09,
        "caps": 0xffe5,
        "shift": 0xffe1,
        "ctrl": 0xffe3,
        "alt": 0xffe9,
        "win": 0xffeb,
        "backspace": 0xff08,
        "enter": 0xff0d,
        "←": 0xff51,
        "↑": 0xff52,
        "→": 0xff53,
        "↓": 0xff54,
        "┗━┛": 0x0020, //space
        "f1": 0xffbe,
        "f2": 0xffbf,
        "f3": 0xffc0,
        "f4": 0xffc1,
        "f5": 0xffc2,
        "f6": 0xffc3,
        "f7": 0xffc4,
        "f8": 0xffc5,
        "f9": 0xffc6,
        "f10": 0xffc7,
        "f11": 0xffc8,
        "f12": 0xffc9
    }
    if(keyCodeList.hasOwnProperty(keyName.toLowerCase())){
        return keyCodeList[keyName.toLowerCase()]
    }else{
        return keyName[0].charCodeAt(0)
    }
}




