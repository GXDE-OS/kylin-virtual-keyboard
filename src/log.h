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

#ifndef LOG_H
#define LOG_H

#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_DEBUG

#include <memory>

#include <QObject>
#include <QPoint>
#include <QSize>
#include <QString>

#include <spdlog/spdlog.h>

#define KVKBD_TRACE(...)                                                       \
    SPDLOG_LOGGER_TRACE(spdlog::default_logger_raw(), __VA_ARGS__)
#define KVKBD_DEBUG(...)                                                       \
    SPDLOG_LOGGER_DEBUG(spdlog::default_logger_raw(), __VA_ARGS__)
#define KVKBD_INFO(...)                                                        \
    SPDLOG_LOGGER_INFO(spdlog::default_logger_raw(), __VA_ARGS__)
#define KVKBD_WARN(...)                                                        \
    SPDLOG_LOGGER_WARN(spdlog::default_logger_raw(), __VA_ARGS__)
#define KVKBD_ERROR(...)                                                       \
    SPDLOG_LOGGER_ERROR(spdlog::default_logger_raw(), __VA_ARGS__)

class SpdlogProxy : public QObject {
    Q_OBJECT
public:
    ~SpdlogProxy() override;
    SpdlogProxy(const SpdlogProxy &) = delete;
    SpdlogProxy &operator=(const SpdlogProxy &) = delete;

    class LogOption {
    public:
        long long fileSize = 1024 * 1024 * 5;
        int fileCounts = 3;
        bool rotateEnable = false;
    };

    static void init(const LogOption &option);
    static void cleanUp();
    static void messageHandler(QtMsgType type,
                               const QMessageLogContext &context,
                               const QString &msg);

    Q_INVOKABLE static void debug(const QString &msg);
    Q_INVOKABLE static void info(const QString &msg);
    Q_INVOKABLE static void warn(const QString &msg);
    Q_INVOKABLE static void error(const QString &msg);

private:
    static QString getGsettingsLogLevel();
    static spdlog::level::level_enum transToSpdLogLevel(const QString &level);
    static QString getWritableLogFilePath();

private:
    static std::shared_ptr<spdlog::logger> m_logger;
};

#endif // SPDLOGPROXY_H
