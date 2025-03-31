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

#include "keyboardserviceproxy.h"

const QString g_serviceName = "org.ukui.Framework";
const QString g_servicePath = "/org/ukui/Framework/Devices/Keyboard";
const char *g_serviceInterface = "org.ukui.Framework.Devices.Keyboard";

const QString g_kbdCountMethodName = "GetKbdCount";
const QString g_kbdStatusSignalName = "KbdCountChanged";

KeyboardServiceProxy::KeyboardServiceProxy(QObject *parent)
    : QDBusAbstractInterface(g_serviceName, g_servicePath, g_serviceInterface,
                             QDBusConnection::sessionBus(), parent) {
    QDBusConnection::sessionBus().connect(
        g_serviceName, g_servicePath, g_serviceInterface, g_kbdStatusSignalName,
        this, SIGNAL(kbdStatusChanged()));
}

QDBusPendingReply<int> KeyboardServiceProxy::GetKbdCount() {
    QList<QVariant> argumentList;

    qDebug() << "KeyboardServiceProxy"
             << "func: " << __FUNCTION__ << " line: ";
    return asyncCallWithArgumentList(g_kbdCountMethodName, argumentList);
}
