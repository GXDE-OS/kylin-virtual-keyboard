import QtQuick 2.0
import "js/utils.js" as Utils
import QtQuick.Window 2.0

Rectangle {
    id: virtualKeyboard
    anchors.fill: parent
    color: virtualKeyboardColor
    radius: placementMode === "EXPANSION" ? 0 : virtualKeyboardFloatPlacementRadius

    //预编辑
    property string preeditText
    //候选词
    property var candidateList
    //可用输入法列表
    property var currentIMList
    //当前输入法名称
    property string uniqueName
    //当前输入法，组成为"uniqueName|localName|label"
    //TOOD 目前以“|”分割数据
    property string currentIM: "||"

    //大小相关
    /*设置默认值，防止报错，fontSize为0时会报错，加载qml文件时height为0，会导致fontSize为0*/
    property real cardinalNumber: height == 0 ? 8 : height/(placementMode === "EXPANSION" ? 64.0 : 68.5)
    property int dragBarHeight: cardinalNumber * 4.5
    property int preeditHeight: cardinalNumber * 5
    property int toolAndCandidateHeight: cardinalNumber * 8
    property int keyboardLayoutHeight: cardinalNumber * 49
    property int keySpacing: cardinalNumber
    property int keyWidth: (width - 5*cardinalNumber) * 2/31 - cardinalNumber
    /*暂时删除win键，将空格键长度从keyWidth * 5.5 + keySpacing * 5修改为keyWidth * 6.5 + keySpacing * 6*/
    property int spaceKeyWidth: keyWidth * 6.5 + keySpacing * 6
    property int keyHeight: cardinalNumber * 9
    property int firstRowKeyHeight: keyHeight * 7/9
    property int keyLableAlignment: keyWidth / 3
    property int keyIconAlignment: keyWidth / 3
    property int imHeight: keyHeight * 2/3
    property int imWidth: keyWidth * 9/5
    property real fontSize: keyHeight * 6/11 * 7/12
    property real shiftFontSize: fontSize / 2
    property real switchKeyFontSize: fontSize * 3/4
    property real fnSymbolKeyFontSize: fontSize * 3/4
    property real actionKeyFontSize: fontSize * 3/4
    property real preeditTextFontSize: fontSize * 3/4
    property real candidateFontSize: fontSize * 3/4
    property real imFontSize: fontSize * 3/4
    property int preeditX: cardinalNumber * 3.5
    property int candidateListWidth: width - 7*cardinalNumber
    property int toolbarSize: cardinalNumber * 3
    property int candidateListX: cardinalNumber * 2
    property int candidateListSpacing: cardinalNumber * 3
    property int keyRadius: cardinalNumber
    property int longPressInterval: 1000
    property int shiftLeftMargin: cardinalNumber
    property int shiftTopMargin: cardinalNumber/2
    property int imLeftMargin: cardinalNumber
    property int dropShadowVerticalOffset: cardinalNumber/4
    property int dragBarIndicatorWidth: cardinalNumber * 7
    property int dragBarIndicatorHeight: cardinalNumber/2

    //颜色相关
    property color virtualKeyboardColor: "#EBEDEF"   /*虚拟键盘背景色*/
    property color charKeyNormalColor: "#FFFFFF"    /*字符按键默认态颜色*/
    property color charKeyPressedColor: "#DDE0E4"   /*字符按键按下态颜色*/
    property color charKeyHoverColor: "#EDF1F5"    /*字符按键悬浮态颜色*/
    property color charKeyDropShadowColor: "#95A0AD"    /*字符按键偏移色*/

    property color actionKeyNormalColor: "#CED3D9"     /*动作按键默认态*/
    property color actionKeyPressedColor: "#A5ACB5"    /*动作按键按下态*/
    property color actionKeyHoverColor:"#C0C6CE"  /*动作按键悬浮态*/

    property color switchKeyNormalColor: "#CED3D9"     /*开关按键默认态*/
    property color switchKeyPressedColor: "#A5ACB5"    /*开关按键按下态*/
    property color switchKeyOpenColor: "#3690F8"   /*开关按键开启态*/
    property color switchKeyOpenPressedColor: "#2C73C6"    /*开关按键开启_按下态*/
    property color switchKeyHoverNormalColor: "#C0C6CE"    /*开关按键默认hover态*/
    property color switchKeyHoverOpenColor: "#328AF0"  /*开关按键按下hover态*/
    property color switchKeyNormalDropShadowColor: "#95A0AD"    /*开关按键默认偏移色*/
    property color switchKeyOpenDropShadowColor: "#1174E5"    /*开关按键开启偏移色*/
    property color switchKeyOpenPressedDropShadowColor: "#165FB3"    /*开关按键开启_按下偏移色*/

    property color currentIMColor: "#328AF0"  /*输入法列表中当前输入法高亮*/
    property color candidateListBackgroundColor: "#DDE0E4"  /*候选词列表背景色*/
    property color preeditBottomColor: "#E4E6E9" /*预编辑下方分割线颜色*/
    property color dragBarIndicatorColor: "#1D1D1D" /*拖拽条指示器颜色*/

    //圆角相关
    property int virtualKeyboardFloatPlacementRadius: 16
    property int dragBarIndicatorRadius: 2

    //状态相关
    property string letterState: "NORMAL"
    property string symbolState: "NORMAL"
    property string fnSymbolState: "NORMAL"
    property string capslockState: "NORMAL"
    property string shiftState: "NORMAL"
    property string altState: "NORMAL"
    property string ctrlState: "NORMAL"
    property string winState: "NORMAL"
    property string changeIMState: "NORMAL"
    property string switchLayoutButtonState: "NORMAL"
    property string placementMode: "EXPANSION"

    //可见性相关
    property bool isToolbarVisible: true
    property bool isToolAreaVisible: false
    property bool isKeyBoardLayoutVisible: true
    property bool isAllLayoutListVisible: false
    property string layout: "classic"
    property bool isCurrentIMListVisible: false

    //内部使用
    signal showToolbar()
    signal showCandidateList()
    signal charKeyClicked()
    signal shiftClicked()
    signal altClicked()
    signal ctrlClicked()
    signal winClicked()

    //后台发送给前台的信号
    signal qmlUpdatePreedit(string preeditText)
    signal qmlUpdateCandidateList(var candidateList)
    signal qmlReset()
    signal qmlChangeIM(string uniqueName)
    signal qmlUpdateCurrentIMList(var currentIMList)
    signal qmlEnterExpansionPlacementMode()
    signal qmlEnterFloatPlacementMode()

    //前台发送给后台的信号
    signal qmlKeyEvent(string key, int keycode, int modifierKeyStates,
                    bool isRelease, int time)
    signal qmlCandidateClicked(int index)
    signal qmlSelectInputMethod(string im)
    signal qmlHideVirtualKeyboard()
    signal qmlRequestCurrentIMList()
    signal qmlSetCurrentIM(string currentIm)
    signal qmlPlacementModeButtonClicked()
    signal qmlMoveBy(int offsetX, int offsetY)
    signal qmlDragEnded()

    Connections {
        target: virtualKeyboard
        onShowToolbar: {
            isToolbarVisible = true
        }
        onShowCandidateList: {
            isToolbarVisible = false
        }
        onQmlUpdatePreedit: (preeditText) => {
            virtualKeyboard.preeditText = preeditText
        }
        onQmlUpdateCandidateList: (candidateList) => {
            if(candidateList.length === 0) {
                showToolbar()
            } else {
                virtualKeyboard.candidateList = candidateList
                showCandidateList()
            }
        }
        onQmlChangeIM: (uniqueName) =>{
            virtualKeyboard.uniqueName = uniqueName
        }
        onQmlUpdateCurrentIMList: (currentIMList)=> {
            virtualKeyboard.currentIMList = currentIMList
        }
        onQmlEnterExpansionPlacementMode: {
            virtualKeyboard.placementMode = "EXPANSION"
        }
        onQmlEnterFloatPlacementMode: {
            virtualKeyboard.placementMode = "FLOAT"
        }
    }

    Component.onCompleted: {
        charKeyClicked.connect(onCharKeyClicked)
        shiftClicked.connect(onShiftClicked)
        ctrlClicked.connect(onCtrlClicked)
        altClicked.connect(onAltClicked)
        winClicked.connect(onWinClicked)
    }

    function onCharKeyClicked() {
        shiftState = "NORMAL"
        ctrlState = "NORMAL"
        altState = "NORMAL"
        winState = "NORMAL"
    }

    function onShiftClicked() {
        ctrlState = "NORMAL"
        altState = "NORMAL"
        winState = "NORMAL"
    }

    function onCtrlClicked() {
        shiftState = "NORMAL"
        altState = "NORMAL"
        winState = "NORMAL"
    }

    function onAltClicked() {
        shiftState = "NORMAL"
        ctrlState = "NORMAL"
        winState = "NORMAL"
    }

    function onWinClicked() {
        shiftState = "NORMAL"
        altState = "NORMAL"
        ctrlState = "NORMAL"
    }

    onUniqueNameChanged: {
        if(currentIMList === undefined){
            qmlRequestCurrentIMList()
        }
        for(var i = 0; i<currentIMList.length; i++){
            if(currentIMList[i].includes(uniqueName)){
                currentIM = currentIMList[i]
            }
        }
    }

    DragBar{id: dragBar}
    Preedit{id: preedit}
    ToolbarAndCandidateArea{id: toolbarAndCandidate}
    KeyboardLayoutArea{
        visible: isKeyBoardLayoutVisible
        anchors.bottom: virtualKeyboard.bottom
        radius: virtualKeyboard.radius
    }
    ToolArea{}

    Rectangle {
        anchors.top: preedit.bottom
        width: virtualKeyboard.width
        height: 1
        color: virtualKeyboard.preeditBottomColor
    }
}
