cmake_minimum_required(VERSION 3.10.0)

project(kylin-virtual-keyboard LANGUAGES CXX)

# set something
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# find packages
find_package(QT NAMES Qt6 Qt5 REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Gui Widgets Concurrent DBus Quick QuickControls2 REQUIRED)
if (NOT Qt${QT_VERSION_MAJOR}_FOUND)
    message(FATAL_ERROR "Qt${QT_VERSION_MAJOR} not found!")
endif()

find_package(KF${QT_VERSION_MAJOR}WindowSystem REQUIRED)
if (NOT KF${QT_VERSION_MAJOR}WindowSystem_FOUND)
    message(FATAL_ERROR "KF${QT_VERSION_MAJOR}WindowSystem not found!")
endif()

find_package(Fcitx5Core REQUIRED)
find_package(Fcitx5Qt${QT_VERSION_MAJOR}DBusAddons REQUIRED)
if ((NOT Fcitx5Core_FOUND) AND (NOT Fcitx5Qt${QT_VERSION_MAJOR}DBusAddons_FOUND))
    message(FATAL_ERROR "Fcitx5Core and Fcitx5Qt${QT_VERSION_MAJOR}DBusAddons are not found!")
endif()

find_package(PkgConfig REQUIRED)
pkg_check_modules(GSettings_QT REQUIRED IMPORTED_TARGET gsettings-qt)

include_directories(${CMAKE_SOURCE_DIR}/src)
include_directories(${GSettings_QT_INCLUDE_DIRS})
include("${FCITX_INSTALL_CMAKECONFIG_DIR}/Fcitx5Utils/Fcitx5CompilerSettings.cmake")

set(HEADERS
    src/animation/animationfactory.h
    src/animation/animator.h
    src/animation/disabledanimator.h
    src/animation/enabledanimator.h
    src/animation/expansionanimationfactory.h
    src/animation/floatanimationfactory.h
    src/appinputareamanager.h
    src/geometrymanager/expansiongeometrymanager.h
    src/geometrymanager/floatgeometrymanager.h
    src/geometrymanager/geometrymanager.h
    src/ipc/dbusservice.h
    src/ipc/fcitxvirtualkeyboardserviceproxy.h
    src/ipc/requestmerger.h
    src/localsettings/localsettings.h
    src/localsettings/viewlocalsettings.h
    src/screenmanager.h
    src/virtualkeyboard/placementmodemanager.h
    src/virtualkeyboard/virtualkeyboardmanager.h
    src/virtualkeyboard/virtualkeyboardmodel.h
    src/virtualkeyboard/virtualkeyboardstrategy.h
    src/virtualkeyboard/virtualkeyboardview.h
    src/virtualkeyboardentry/fcitxvirtualkeyboardservice.h
    src/virtualkeyboardentry/floatbutton.h
    src/virtualkeyboardentry/floatbuttonmanager.h
    src/virtualkeyboardentry/floatbuttonstrategy.h
    src/virtualkeyboardentry/virtualkeyboardentrymanager.h
    src/virtualkeyboardentry/virtualkeyboardtrayicon.h
    src/virtualkeyboardsettings/virtualkeyboardsettings.h
)

set(SOURCES
    src/animation/animationfactory.cpp
    src/animation/disabledanimator.cpp
    src/animation/enabledanimator.cpp
    src/animation/expansionanimationfactory.cpp
    src/animation/floatanimationfactory.cpp
    src/appinputareamanager.cpp
    src/geometrymanager/expansiongeometrymanager.cpp
    src/geometrymanager/floatgeometrymanager.cpp
    src/geometrymanager/geometrymanager.cpp
    src/ipc/dbusservice.cpp
    src/ipc/fcitxvirtualkeyboardserviceproxy.cpp
    src/ipc/requestmerger.cpp
    src/localsettings/viewlocalsettings.cpp
    src/main.cpp
    src/screenmanager.cpp
    src/virtualkeyboard/placementmodemanager.cpp
    src/virtualkeyboard/virtualkeyboardmanager.cpp
    src/virtualkeyboard/virtualkeyboardmodel.cpp
    src/virtualkeyboard/virtualkeyboardview.cpp
    src/virtualkeyboard/virtualkeyboardviewstate.cpp
    src/virtualkeyboardentry/floatbutton.cpp
    src/virtualkeyboardentry/floatbuttonmanager.cpp
    src/virtualkeyboardentry/virtualkeyboardentrymanager.cpp
    src/virtualkeyboardentry/virtualkeyboardtrayicon.cpp
    src/virtualkeyboardsettings/virtualkeyboardsettings.cpp
)

set(TRANSLATIONS
    translations/translation.ts
    translations/translation_bo_CN.ts
    translations/translation_en.ts
    translations/translation_zh_CN.ts
)

set(RESOURCES
    floatbutton.qrc
    qml.qrc
    translations.qrc
)

add_executable(${PROJECT_NAME} ${HEADERS} ${SOURCES} ${RESOURCES} ${TRANSLATIONS})
target_link_libraries(${PROJECT_NAME}
                Qt${QT_VERSION_MAJOR}::Core
                Qt${QT_VERSION_MAJOR}::Gui
                Qt${QT_VERSION_MAJOR}::Widgets
                Qt${QT_VERSION_MAJOR}::Concurrent
                Qt${QT_VERSION_MAJOR}::DBus
                Qt${QT_VERSION_MAJOR}::Quick
                Qt${QT_VERSION_MAJOR}::QuickControls2
                KF${QT_VERSION_MAJOR}::WindowSystem
                Fcitx5::Core
                Fcitx5Qt${QT_VERSION_MAJOR}::DBusAddons
                ${GSettings_QT_LIBRARIES}
            )

install(TARGETS ${PROJECT_NAME}  DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
install(FILES data/org.ukui.virtualkeyboard.gschema.xml DESTINATION ${CMAKE_INSTALL_PREFIX}/share/glib-2.0/schemas/)
install(FILES data/ukui-virtual-keyboard-symbolic.svg DESTINATION ${CMAKE_INSTALL_PREFIX}/share/icons/hicolor/scalable/actions)
install(FILES data/kylin-virtual-keyboard.desktop DESTINATION /etc/xdg/autostart/)
install(FILES data/kylin-virtual-keyboard-xwayland DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
