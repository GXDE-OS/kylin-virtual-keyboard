QT += quick
QT += dbus
QT += core gui
QT += KWindowSystem
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    src/appinputareamanager.cpp \
    src/dbusservice.cpp \
    src/eventhandler.cpp \
    src/expansiongeometrymanager.cpp \
    src/floatgeometrymanager.cpp \
    src/geometrymanager.cpp \
    src/main.cpp \
    src/requestmerger.cpp \
    src/screenmanager.cpp \
    src/trayentry.cpp \
    src/placementmodemanager.cpp \
    src/viewlocalsettings.cpp \
    src/virtualkeyboardmanager.cpp \
    src/virtualkeyboardview.cpp

TRANSLATIONS = translations/translation.ts \
    translations/translation_bo_CN.ts \
    translations/translation_en.ts \
    translations/translation_zh_CN.ts

RESOURCES += \
    qml.qrc \
    translations.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /usr/bin
else: unix:!android: target.path = /usr/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    src/appinputareamanager.h \
    src/dbusservice.h \
    src/eventhandler.h \
    src/expansiongeometrymanager.h \
    src/floatgeometrymanager.h \
    src/geometrymanager.h \
    src/localsettings.h \
    src/placementmodemanager.h \
    src/requestmerger.h \
    src/screenmanager.h \
    src/viewlocalsettings.h \
    src/virtualkeyboardmanager.h \
    src/trayentry.h \
    src/virtualkeyboardview.h

DISTFILES +=

LIBS += -lFcitx5Qt5DBusAddons

