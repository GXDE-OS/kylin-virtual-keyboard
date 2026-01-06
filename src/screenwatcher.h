/*
 * Copyright (c) KylinSoft Co., Ltd. 2025.All rights reserved.
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

#ifndef SCREENWATCHER_H
#define SCREENWATCHER_H

#include <QGuiApplication>
#include <QHash>
#include <QObject>
#include <QPoint>
#include <QRect>
#include <QScreen>
#include <QString>

/**
 * @brief 屏幕信息结构体
 */
struct ScreenInfo {
    QScreen *screen = nullptr; // 屏幕指针
    QString name;              // 屏幕名称
    QRect geometry;            // 屏幕几何
    bool hasWindow = false;    // 窗口是否显示在这个屏幕上

    ScreenInfo() = default;
    ScreenInfo(QScreen *s, const QString &n, const QRect &g)
        : screen(s), name(n), geometry(g), hasWindow(false) {}

    bool hasWindowMark() const { return hasWindow; }
};

class ScreenWatcher : public QObject {
    Q_OBJECT

public:
    static ScreenWatcher &getInstance();

    QRect getPrimaryScreenGeometry() const;

    /**
     * @brief 获取最合适的屏幕几何
     * @param position 位置点
     * @return 屏幕几何矩形
     *
     * 优先级：
     * 1. 位置有效对应的屏幕几何
     * 2. 虚拟键盘正在显示的屏幕几何
     * 3. 主屏幕几何
     */
    QRect getOptimalScreenGeometry(const QPoint &position) const;

    /**
     * @brief 标记虚拟键盘窗口所在的屏幕
     * @param position 位置点
     * @return
     *
     * 虚拟键盘有三种模式ui: 悬浮模式，平铺模式，悬浮球
     * 同一时间，只会显示一种模式的ui
     * 显示时，基于窗口最新的位置来标记窗口所在的屏幕
     * 当屏幕改变时，直接基于标记来决定是否向键盘管理器发送信号
     */
    void markScreen(const QPoint &position);

signals:
    void screensChanged();

private:
    explicit ScreenWatcher(QObject *parent = nullptr);
    ~ScreenWatcher() override = default;

    Q_DISABLE_COPY(ScreenWatcher)

    void connectScreenSignals();
    // 屏幕对象移除或新增后，需要重新连接对应的几何信号
    void connectScreenGeometryChanged(QScreen *screen);

private slots:
    void onScreenAdded(QScreen *screen);
    void onScreenRemoved(QScreen *screen);
    void onScreenGeometryChanged(const QRect &geometry);
    void onPrimaryScreenChanged(QScreen *screen);

private:
    // 判断屏幕是否有效
    // qt会在没有屏幕时，添加一个假的屏幕
    // ScreenWatcher直接过滤这个无用屏幕的相关信号
    bool isScreenValid(const QScreen *screen) const;
    // 判断屏幕是否被虚拟键盘标记过
    bool isScreenMarked(const QScreen *screen) const;
    void updateScreenList();
    ScreenInfo *getScreenInfo(QScreen *screen);
    const ScreenInfo *getScreenInfoConst(QScreen *screen) const;
    // 是否有屏幕被虚拟键盘显示时标记
    bool isAnyScreenMarked() const;

private:
    QString lastPrimaryScreenName_;
    QHash<QScreen *, ScreenInfo> screenList_;
};

#endif // SCREENWATCHER_H
