#include <QApplication>
#include "virtualkeyboardmanager.h"

int main(int argc, char *argv[]) {
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication app(argc, argv);
    VirtualKeyboardManager virtualKeyboardManager;
    return app.exec();
}
