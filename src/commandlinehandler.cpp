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

#include "commandlinehandler.h"
#include <iostream>
#include "log.h"
#include "messagehandler.h"

CommandLineHandler::CommandLineHandler()
    : m_loglevelOption(QStringList() << "loglevel",
                       "Set the log level \n"
                       "(available: debug, info, warn, error).\n"
                       "e.g. --loglevel=debug",
                       "loglevel") {
    m_parser.addHelpOption();
    m_parser.addVersionOption();
    m_parser.addOption(m_loglevelOption);
}

void CommandLineHandler::parseArguments(QtSingleApplication &app) {
    m_parser.process(app);
}

bool CommandLineHandler::shouldContinueExecution(QtSingleApplication &app) {
    // 单实例判断
    if (app.isRunning()) {
        if (m_parser.isSet(m_loglevelOption)) {
            QString level = m_parser.value(m_loglevelOption);
            QString command = QString("loglevel %1").arg(level);
            app.sendMessage(command);
            std::cout << "Log level change request sent to main instance."
                      << std::endl;
            return false;
        }
        std::cout << "kylin-virtual-keyboard is already running!" << std::endl;
        return false;
    }

    return true;
}

void CommandLineHandler::bindMessageHandler(QtSingleApplication &app,
                                            MessageHandler &messageHandler) {
    QObject::connect(
        &app, &QtSingleApplication::messageReceived,
        [&](const QString &message) {
            messageHandler.processMessage(message, [&](const QString &result) {
                KVKBD_INFO("Command result: {}", result.toStdString());
            });
        });
}
