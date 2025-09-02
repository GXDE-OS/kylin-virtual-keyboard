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

#include <csignal>
#include <iostream>
#include <QLocale>
#include <QTranslator>

#include "ipc/dbusservice.h"
#include "ipc/fcitxvirtualkeyboardserviceproxy.h"
#include "log.h"
#include "messagehandler.h"
#include "qtsingleapplication/src/QtSingleApplication"
#include "virtualkeyboard/virtualkeyboardmanager.h"
#include "virtualkeyboardentry/virtualkeyboardentrymanager.h"

const QString APP_ID = "kylin-virtual-keyboard";

void signalHandler(int sig) {
    const char *name = "UNKNOWN";
    switch (sig) {
    case SIGSEGV:
        name = "SIGSEGV";
        break;
    case SIGFPE:
        name = "SIGFPE";
        break;
    case SIGILL:
        name = "SIGILL";
        break;
    case SIGABRT:
        name = "SIGABRT";
        break;
    case SIGINT:
        name = "SIGINT";
        break;
    case SIGTERM:
        name = "SIGTERM";
        break;
    default:
        break;
    }

    KVKBD_ERROR("SIGNAL: {} ({})", name, sig);
    spdlog::shutdown();
    std::signal(sig, SIG_DFL);
    std::raise(sig);
}

void installCrashHandler() {
    std::signal(SIGSEGV, signalHandler);
    std::signal(SIGFPE, signalHandler);
    std::signal(SIGILL, signalHandler);
    std::signal(SIGABRT, signalHandler);
    std::signal(SIGINT, signalHandler);
    std::signal(SIGTERM, signalHandler);
}

int main(int argc, char *argv[]) {
    QtSingleApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QtSingleApplication app(APP_ID, argc, argv);
    QtSingleApplication::setApplicationName(APP_ID);

    // 命令行解析器
    QCommandLineParser parser;
    QCommandLineOption loglevelOption(QStringList() << "loglevel",
                                      "Set the log level \n"
                                      "(available: debug, info, warn, error).\n"
                                      "e.g. --loglevel=debug",
                                      "loglevel");
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addOption(loglevelOption);
    parser.process(app);

    // 单例判断
    if (app.isRunning()) {
        if (parser.isSet(loglevelOption)) {
            QString level = parser.value(loglevelOption);
            QString command = QString("loglevel %1").arg(level);
            app.sendMessage(command);
            std::cout << "Log level change request sent to main instance."
                      << std::endl;
            return 0;
        }
        std::cout << APP_ID.toStdString() << " is already running!"
                  << std::endl;
        return 0;
    }

    // 初始化日志模块
    SpdlogProxy::LogOption logOption;
    SpdlogProxy::init(logOption);
    KVKBD_INFO("{},---START---", APP_ID.toStdString());

    // 注册异常信号处理器
    installCrashHandler();
    // 单例消息处理器
    MessageHandler messageHandler;

    // 初次运行
    if (argc > 1) {
        if (parser.isSet(loglevelOption)) {
            QString level = parser.value(loglevelOption);
            QString command = QString("loglevel %1").arg(level);
            messageHandler.processMessage(command, [&](const QString &result) {
                KVKBD_INFO("Command result: {}", result.toStdString());
            });
        }
    }

    // 绑定接收二次运行时发送的消息
    QObject::connect(
        &app, &QtSingleApplication::messageReceived,
        [&](const QString &message) {
            messageHandler.processMessage(message, [&](const QString &result) {
                KVKBD_INFO("Command result: {}", result.toStdString());
            });
        });

    QObject::connect(&app, &QtSingleApplication::aboutToQuit, []() {
        KVKBD_INFO("Application exited normally");
        spdlog::shutdown();
    });

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
