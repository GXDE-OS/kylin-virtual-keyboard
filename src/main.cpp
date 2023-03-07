#include <QApplication>
#include <QLocale>
#include <QTranslator>

#include "trayentry.h"
#include "virtualkeyboardmanager.h"

int main(int argc, char *argv[]) {
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication app(argc, argv);

    QTranslator translator;
    if (translator.load(QLocale::system(), "translation", "_",
                        ":/translations")) {
        app.installTranslator(&translator);
    }

    VirtualKeyboardManager virtualKeyboardManager;
    TrayEntry trayEntry(&virtualKeyboardManager);

    return app.exec();
}
