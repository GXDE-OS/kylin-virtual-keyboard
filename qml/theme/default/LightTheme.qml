/*
* Copyright 2025 KylinSoft Co., Ltd.
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

import "../../js/themeutils.js" as ThemeUtils
import QtQuick 2.0

QtObject {
    id: lightTheme

    // 虚拟键盘背景色
    property color virtualKeyboardColor: "#F6F6F6"
    // 字符按键颜色
    property color charKeyNormalColor: "#FFFFFF"
    property color charKeyPressedColor: Qt.rgba(0, 0, 0, 0.2)
    property color charKeyHoverColor: Qt.rgba(0, 0, 0, 0.15)
    property color charKeyDropShadowColor: Qt.rgba(115 / 255, 115 / 255, 115 / 255, 0.8)
    // 动作按键颜色
    property color actionKeyNormalColor: Qt.rgba(0, 0, 0, 0.1)
    property color actionKeyPressedColor: Qt.rgba(0, 0, 0, 0.2)
    property color actionKeyHoverColor: Qt.rgba(0, 0, 0, 0.15)
    // 开关按键颜色
    property color switchKeyNormalColor: Qt.rgba(0, 0, 0, 0.1)
    property color switchKeyPressedColor: Qt.rgba(0, 0, 0, 0.2)
    property color switchKeyHoverNormalColor: Qt.rgba(0, 0, 0, 0.15)
    property color switchKeyOpenColor: "#3676F5"
    // KBrand-Click: linear-gradient(0deg, rgba(0, 0, 0, 0.2), rgba(0, 0, 0, 0.2)), #3790FA
    property color switchKeyOpenPressedColor: ThemeUtils.calculateMixedColor(Qt.rgba(55 / 255, 144 / 255, 250 / 255, 1), 0.2)
    // KBrand-Hover: linear-gradient(0deg, rgba(0, 0, 0, 0.05), rgba(0, 0, 0, 0.05)), #3790FA
    property color switchKeyHoverOpenColor: ThemeUtils.calculateMixedColor(Qt.rgba(55 / 255, 144 / 255, 250 / 255, 1), 0.05)
    // 开关按键阴影颜色
    property color switchKeyNormalDropShadowColor: Qt.rgba(115 / 255, 115 / 255, 115 / 255, 0.8)
    property color switchKeyOpenDropShadowColor: Qt.rgba(3 / 255, 59 / 255, 170 / 255, 0.8)
    property color switchKeyOpenPressedDropShadowColor: Qt.rgba(115 / 255, 115 / 255, 115 / 255, 0.8)
    // 输入法列表高亮颜色
    property color currentIMColor: "#3676F5"
    // 候选词相关颜色
    property color candidateListBackgroundColor: "#E6E6E6"
    property color candidateDefaultColor: Qt.rgba(0, 0, 0, 0.85)
    property color candidateHighlightColor: "#3790FA"
    property color preeditBottomColor: Qt.rgba(0, 0, 0, 0.1)
    // 拖拽条
    property color dragBarIndicatorColor: Qt.rgba(0, 0, 0, 0.85)
    // 关闭按钮
    // KError-Click: linear-gradient(0deg, rgba(0, 0, 0, 0.2), rgba(0, 0, 0, 0.2)), #F53F3F
    property color hideButtonPressedColor: ThemeUtils.calculateMixedColor(Qt.rgba(245 / 255, 73 / 255, 73 / 255, 1), 0.2)
    // KError-Hover: linear-gradient(0deg, rgba(0, 0, 0, 0.05), rgba(0, 0, 0, 0.05)), #F53F3F
    property color hideButtonHoverColor: ThemeUtils.calculateMixedColor(Qt.rgba(245 / 255, 73 / 255, 73 / 255, 1), 0.05)
    // 模式切换
    property color placementButtonPressedColor: Qt.rgba(0, 0, 0, 0.2)
    property color placementButtonHoverColor: Qt.rgba(0, 0, 0, 0.15)
    // 文本颜色
    property color fontPrimaryColor: Qt.rgba(0, 0, 0, 0.85)
    property color fontSecondaryColor: Qt.rgba(0, 0, 0, 0.35)
    // 圆角属性
    property int virtualKeyboardFloatPlacementRadius: 12
    property int dragBarIndicatorRadius: 2
    property int toolbarRadius: 8
}
