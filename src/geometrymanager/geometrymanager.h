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

#ifndef GEOMETRYMANAGER_H
#define GEOMETRYMANAGER_H

#include <QObject>
#include <QPoint>
#include <QSize>

class VirtualKeyboardManager;

class GeometryManager : public QObject {
    Q_OBJECT
public:
    ~GeometryManager() override = default;

public slots:
    void updateGeometry();

signals:
    void viewMoved(int x, int y);
    void viewResized(int width, int height);

protected:
    GeometryManager();

    QSize calculateViewSize() const;

private:
    virtual int calculateViewWidth() const = 0;
    virtual int calculateViewHeight() const = 0;

    virtual QPoint calculateViewPosition() const = 0;
};

#endif // GEOMETRYMANAGER_H
