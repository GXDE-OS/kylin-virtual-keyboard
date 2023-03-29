#include <QApplication>
#include <QLocale>
#include <QTranslator>

#include "ipc/dbusservice.h"
#include "ipc/fcitxvirtualkeyboardserviceproxy.h"
#include "virtualkeyboard/virtualkeyboardmanager.h"
#include "virtualkeyboardentry/trayentry.h"

int main(int argc, char *argv[]) {
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication app(argc, argv);

    QTranslator translator;
    if (translator.load(QLocale::system(), "translation", "_",
                        ":/translations")) {
        app.installTranslator(&translator);
    }

    VirtualKeyboardManager virtualKeyboardManager;
    FcitxVirtualKeyboardServiceProxy virtualKeyboardService;
    TrayEntry trayEntry(virtualKeyboardService, virtualKeyboardManager);

    DBusService dbusService(&virtualKeyboardManager);

    return app.exec();
}
