cmake_minimum_required(VERSION 3.10.0)

project(kylin-virtual-keyboard LANGUAGES CXX)

OPTION(ENABLE_TEST "Build Test" OFF)

# set something
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# find packages
find_package(spdlog REQUIRED)
if (NOT spdlog_FOUND)
    message(FATAL_ERROR "Spdlog not found!")
endif()


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

include_directories(${GSettings_QT_INCLUDE_DIRS})
include("${FCITX_INSTALL_CMAKECONFIG_DIR}/Fcitx5Utils/Fcitx5CompilerSettings.cmake")

set(TRANSLATIONS
    ${CMAKE_SOURCE_DIR}/translations/translation.ts
    ${CMAKE_SOURCE_DIR}/translations/translation_bo_CN.ts
    ${CMAKE_SOURCE_DIR}/translations/translation_en.ts
    ${CMAKE_SOURCE_DIR}/translations/translation_zh_CN.ts
    ${CMAKE_SOURCE_DIR}/translations/translation_zh_HK.ts
    ${CMAKE_SOURCE_DIR}/translations/translation_mn.ts
    ${CMAKE_SOURCE_DIR}/translations/translation_kk.ts
    ${CMAKE_SOURCE_DIR}/translations/translation_ug.ts
    ${CMAKE_SOURCE_DIR}/translations/translation_ky.ts
)

set(RESOURCES
    ${CMAKE_SOURCE_DIR}/floatbutton.qrc
    ${CMAKE_SOURCE_DIR}/qml.qrc
    ${CMAKE_SOURCE_DIR}/translations.qrc
)

add_subdirectory(third-party)
add_subdirectory(data)
add_subdirectory(src)
if (ENABLE_TEST)
    enable_testing()
    add_subdirectory(test)
endif()
