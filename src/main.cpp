#include <QApplication>
#include "virtualkeyboardmanager.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    VirtualKeyboardManager virtualKeyboardManager;
    return app.exec();
}
