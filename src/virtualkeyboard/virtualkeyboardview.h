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

#ifndef VIRTUALKEYBOARDVIEW_H
#define VIRTUALKEYBOARDVIEW_H

#include <memory>

#include <QObject>
#include <QQuickView>
#include <QString>

class VirtualKeyboardView : public QObject {
    Q_OBJECT

public:
    VirtualKeyboardView(QObject &manager, QObject &model);
    ~VirtualKeyboardView() override;

    QRect geometry() const;

    bool isVisible() const;
    void show();
    void hide();

signals:
    void updatePreeditArea(const QString &preeditText);
    void updateCandidateArea(const QVariant &candidateTextList,
                             int globalCursorIndex);
    void imDeactivated();

    void expansionModeEntered();
    void floatModeEntered();

public slots:
    void move(int x, int y);
    void resize(int width, int height);

private:
    void initView();
    void connectSignals();
    void destroyView();

private:
    QObject &manager_;
    QObject &model_;
    std::unique_ptr<QQuickView> view_ = nullptr;
};

#endif // VIRTUALKEYBOARDVIEW_H
