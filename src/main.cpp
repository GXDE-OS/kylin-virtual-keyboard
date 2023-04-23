#include <QApplication>
#include <QLocale>
#include <QTranslator>

#include "ipc/dbusservice.h"
#include "ipc/fcitxvirtualkeyboardserviceproxy.h"
#include "virtualkeyboard/virtualkeyboardmanager.h"
#include "virtualkeyboardentry/virtualkeyboardentrymanager.h"

int main(int argc, char *argv[]) {
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication app(argc, argv);

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
