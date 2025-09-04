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

#include "errorhandler.h"

#include <QDir>
#include <QStandardPaths>
#include <QString>
#include <QStringList>

#include <fcntl.h>
#include <signal.h>
#include <unistd.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <execinfo.h>

#include <iostream>

#define MINIMAL_BUFFER_SIZE 256
#define BACKTRACE_SIZE 32

QString ErrorHandler::s_crashLogPath;
bool ErrorHandler::s_initialized = false;
const QString fileName = "kylin-virtual-keyboard-error.log";

void ErrorHandler::init() {
    if (s_initialized) {
        return;
    }

    QString homeDir =
        QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
    QStringList logPaths;
    QString primaryLogPath = homeDir + "/.log/";
    QString fallbackLogPath =
        QStandardPaths::writableLocation(QStandardPaths::TempLocation) +
        "/kylin-virtual-keyboard/";
    logPaths << primaryLogPath << fallbackLogPath;

    QString crashLogPath;
    bool logDirWritable = false;

    for (const QString &logPath : logPaths) {
        QDir logDir(logPath);

        if (logDir.exists()) {
            if (QFileInfo(logPath).isWritable()) {
                crashLogPath = logPath + fileName;
                logDirWritable = true;
                break;
            }
        } else {
            if (logDir.mkpath(".")) {
                crashLogPath = logPath + fileName;
                logDirWritable = true;
                break;
            }
        }
    }

    if (!logDirWritable) {
        QString tempPath =
            QStandardPaths::writableLocation(QStandardPaths::TempLocation);
        crashLogPath = tempPath + "/" + fileName;
    }

    setCrashLogPath(crashLogPath);

    // 注册信号处理器
    int signo;
    for (signo = SIGHUP; signo < SIGUNUSED; signo++) {
        switch (signo) {
        case SIGTSTP:
        case SIGCONT:
            continue;
        case SIGALRM:
        case SIGPIPE:
        case SIGUSR2:
        case SIGWINCH:
        case SIGURG:
            signal(signo, SIG_IGN);
            break;
        default:
            signal(signo, signalHandler);
        }
    }

    void *array[BACKTRACE_SIZE] = {
        nullptr,
    };
    (void)backtrace(array, BACKTRACE_SIZE);

    s_initialized = true;
}

void ErrorHandler::setCrashLogPath(const QString &path) {
    s_crashLogPath = path;
}

QString ErrorHandler::getCrashLogPath() { return s_crashLogPath; }

void ErrorHandler::signalHandler(int sig) {
    int fd = -1;

    if (sig == SIGSEGV || sig == SIGABRT || sig == SIGFPE || sig == SIGILL) {
        if (!s_crashLogPath.isEmpty()) {
            fd = open(s_crashLogPath.toLocal8Bit().constData(),
                      O_WRONLY | O_CREAT | O_TRUNC, 0600);
        }
    }

    writeString(fd, "=========================\n");
    writeString(fd, "Kylin Virtual Keyboard -- Get Signal No.: ");
    writeUInt64(fd, sig);
    writeString(fd, " (");
    writeString(fd, getSignalName(sig));
    writeString(fd, ")\n");

    time_t t = time(nullptr);
    writeString(fd, "Date: ");
    writeUInt64(fd, t);
    writeString(fd, " (try \"date -d @");
    writeUInt64(fd, t);
    writeString(fd, "\" if you are using GNU date)\n");

    writeString(fd, "ProcessID: ");
    writeUInt64(fd, getpid());
    writeString(fd, "\n");

    writeString(fd, "\nTo get detailed line information, use:\n");
    writeString(
        fd, "addr2line -e /usr/bin/kylin-virtual-keyboard -f -C <address>\n");

    generateBacktrace(fd);

    if (fd >= 0) {
        close(fd);
    }

    signal(sig, SIG_DFL);
    raise(sig);
}

void ErrorHandler::writeString(int fd, const char *str) {
    if (fd >= 0) {
        writeAll(fd, str, strlen(str));
    }
    //    writeAll(STDERR_FILENO, str, strlen(str));
}

void ErrorHandler::writeBuffer(int fd, const char *buffer, int len) {
    if (fd >= 0) {
        writeAll(fd, buffer, len);
    }
    //    writeAll(STDERR_FILENO, buffer, len);
}

void ErrorHandler::writeUInt64(int fd, unsigned long long number) {
    char buffer[MINIMAL_BUFFER_SIZE]{0};
    int len = snprintf(buffer, sizeof(buffer), "%llu", number);
    if (len > 0 && len < (int)sizeof(buffer)) {
        writeBuffer(fd, buffer, len);
    }
}

ssize_t ErrorHandler::writeAll(int fd, const void *buf, size_t count) {
    const char *p = (const char *)buf;
    while (count > 0) {
        ssize_t written = write(fd, p, count);
        if (written == -1) {
            if (errno == EINTR)
                continue;
            return -1;
        }
        p += written;
        count -= written;
    }
    return 0;
}

const char *ErrorHandler::getSignalName(int sig) {
    switch (sig) {
    case SIGSEGV:
        return "SIGSEGV";
    case SIGFPE:
        return "SIGFPE";
    case SIGILL:
        return "SIGILL";
    case SIGABRT:
        return "SIGABRT";
    case SIGINT:
        return "SIGINT";
    case SIGTERM:
        return "SIGTERM";
    case SIGBUS:
        return "SIGBUS";
    case SIGPIPE:
        return "SIGPIPE";
    default:
        return "UNKNOWN";
    }
}

void ErrorHandler::generateBacktrace(int fd) {
    void *array[BACKTRACE_SIZE]{
        nullptr,
    };
    int size = backtrace(array, BACKTRACE_SIZE);

    writeString(fd, "\nBacktrace:\n");
    writeString(fd, "Backtrace size: ");
    writeUInt64(fd, size);
    writeString(fd, "\n");
    writeString(fd, "\nBacktrace addresses:\n");

    //    backtrace_symbols_fd(array, size, STDERR_FILENO);
    if (fd >= 0) {
        backtrace_symbols_fd(array, size, fd);
    }
}
