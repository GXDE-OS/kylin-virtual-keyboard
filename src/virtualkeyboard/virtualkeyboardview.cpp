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

#include "virtualkeyboardview.h"

#include <QQmlContext>
#include <QQuickItem>

VirtualKeyboardView::VirtualKeyboardView(QObject *manager, QObject *model)
    : view_(new QQuickView()) {
    init(manager, model);

    connectSignals();

    view_->show();
}

VirtualKeyboardView::~VirtualKeyboardView() {
    if (view_ == nullptr) {
        return;
    }

    if (view_->isVisible()) {
        view_->hide();
    }

    view_.reset();
}

QRect VirtualKeyboardView::geometry() const { return view_->geometry(); }

void VirtualKeyboardView::move(int x, int y) {
    view_->setX(x);
    view_->setY(y);
}

void VirtualKeyboardView::resize(int width, int height) {
    view_->setWidth(width);
    view_->setHeight(height);
}

void VirtualKeyboardView::init(QObject *manager, QObject *model) {
    view_->rootContext()->setContextProperty("manager", manager);
    view_->rootContext()->setContextProperty("model", model);

    view_->setColor(QColor(Qt::transparent));
    view_->setSource(QUrl("qrc:/qml/VirtualKeyboard.qml"));
    view_->setFlags(Qt::Window | Qt::WindowDoesNotAcceptFocus |
                    Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint |
                    Qt::BypassWindowManagerHint);
}

void VirtualKeyboardView::connectSignals() {
    auto *rootObject = view_->rootObject();

    connect(this, SIGNAL(updatePreeditArea(const QString &)), rootObject,
            SIGNAL(qmlUpdatePreedit(QString)));
    connect(this, SIGNAL(updateCandidateArea(const QVariant &, int)),
            rootObject, SIGNAL(qmlUpdateCandidateList(QVariant, int)));
    connect(this, SIGNAL(imDeactivated()), rootObject,
            SIGNAL(qmlImDeactivated()));

    connect(this, SIGNAL(expansionModeEntered()), rootObject,
            SIGNAL(qmlEnterExpansionPlacementMode()));
    connect(this, SIGNAL(floatModeEntered()), rootObject,
            SIGNAL(qmlEnterFloatPlacementMode()));
}
