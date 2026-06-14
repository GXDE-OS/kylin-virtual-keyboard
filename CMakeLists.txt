cmake_minimum_required(VERSION 3.16.0)

project(kylin-virtual-keyboard LANGUAGES CXX)

OPTION(ENABLE_TEST "Build Test" OFF)

# 自动处理 Qt 元对象编译和资源文件
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 查找依赖包
find_package(spdlog REQUIRED)
if(NOT spdlog_FOUND)
    message(FATAL_ERROR "Spdlog not found!")
endif()

# 强制使用 Qt6（移除 ENALBE_QT6 选项及 Qt5 分支）
find_package(Qt6 COMPONENTS Core Gui Widgets Concurrent DBus Quick QuickControls2 Core5Compat REQUIRED)
message(STATUS "Current Qt version: ${Qt6_VERSION}")

# KDE Frameworks 6 窗口系统（对应 Qt6 的 KF6）
find_package(KF6WindowSystem REQUIRED)
if(NOT KF6WindowSystem_FOUND)
    message(FATAL_ERROR "KF6WindowSystem not found!")
endif()

find_package(Fcitx5Core REQUIRED)
if(NOT Fcitx5Core_FOUND)
    message(FATAL_ERROR "Fcitx5Core not found!")
endif()

find_package(PkgConfig REQUIRED)
pkg_check_modules(GSettings_QT REQUIRED IMPORTED_TARGET gsettings-qt6)

include(GNUInstallDirs)
include_directories(${GSettings_QT_INCLUDE_DIRS})
include("${FCITX_INSTALL_CMAKECONFIG_DIR}/Fcitx5Utils/Fcitx5CompilerSettings.cmake")

set(RESOURCES
    ${CMAKE_SOURCE_DIR}/resources.qrc
)

add_subdirectory(third-party)
add_subdirectory(data)
add_subdirectory(src)
if(ENABLE_TEST)
    enable_testing()
    add_subdirectory(test)
endif()