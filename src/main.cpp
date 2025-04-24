/*
 * Copyright (c) KylinSoft Co., Ltd. 2022.All rights reserved.
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

#include <QLocale>
#include <QTranslator>
#include <QtSingleApplication>

#include "ipc/dbusservice.h"
#include "ipc/fcitxvirtualkeyboardserviceproxy.h"
#include "virtualkeyboard/virtualkeyboardmanager.h"
#include "virtualkeyboardentry/virtualkeyboardentrymanager.h"

const QString APP_ID = "kylin-virtual-keyboard";


int main(int argc, char *argv[]) {
    QtSingleApplication app(APP_ID, argc, argv);
    if (app.isRunning()) {
        qWarning() << APP_ID << "is already running!";
        QStringList args = QtSingleApplication::arguments();
        args.removeFirst();
        app.sendMessage(args.join(" "));
        return 0;
    }

    // 初次运行
    if (argc > 1) {
        auto message = QtSingleApplication::arguments().join(" ");
        qDebug() << APP_ID << ",Received Message:" << message;
        // TODO: 解析参数，做对应处理
    }
    // TODO：绑定二次运行时，单例接收消息
    //connect(&app, &QtSingleApplication::messageReceived, this, &handleMessage);

    QtSingleApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QTranslator translator;
    if (translator.load(QLocale::system(), "translation", "_",
                        ":/translations")) {
        app.installTranslator(&translator);
    }

    FcitxVirtualKeyboardServiceProxy virtualKeyboardService;
    VirtualKeyboardManager virtualKeyboardManager([&virtualKeyboardService]() {
        virtualKeyboardService.hideVirtualKeyboard();
    });
    VirtualKeyboardEntryManager entryManager(virtualKeyboardManager,
                                             virtualKeyboardService);

    DBusService dbusService(&virtualKeyboardManager);

    return app.exec();
}
