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

#include "expansiongeometrymanager.h"

#include "screenmanager.h"

ExpansionGeometryManager::ExpansionGeometryManager() : GeometryManager() {}

int ExpansionGeometryManager::calculateViewWidth() const {
    return ScreenManager::getPrimaryScreenSize().width();
}

int ExpansionGeometryManager::calculateViewHeight() const {
    QSize viewPortSize = ScreenManager::getPrimaryScreenSize();

    if (viewPortSize.width() > viewPortSize.height()) {
        return viewPortSize.width() * viewHeightRatio_;
    } else {
        // 竖屏情况下，从屏幕左上角开始算起，以当前屏幕宽度为测量基准，
        // 在该范围里面的部分可以正常刷新；在该范围外面部分会出现重影。
        // 因此，需要将虚拟键盘的高度设置为屏幕宽高差值+1，将其作为虚拟
        // 键盘的最小高度,从而确保虚拟键盘可以正确刷新qml内容, 防止旋转
        // 到横屏后出现重影
        // TODO(linyuxuan): 找出bug成因，彻底解决该问题
        const int leastHeight =
            viewPortSize.height() - viewPortSize.width() + 1;
        const int requiredHeight = viewPortSize.height() * viewHeightRatio_;
        return std::max(leastHeight, requiredHeight);
    }
}

QPoint ExpansionGeometryManager::calculateViewPosition() {
    QSize viewPortSize = ScreenManager::getPrimaryScreenSize();

    return QPoint(0, viewPortSize.height() - calculateViewHeight());
}
