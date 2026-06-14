cmake_minimum_required(VERSION 3.16)
project(kylin-virtual-keyboard-test VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 COMPONENTS Core Gui Widgets Concurrent DBus Quick QuickControls2 REQUIRED)

find_package(Gettext REQUIRED)
find_package(Fcitx5Core REQUIRED)
find_package(Fcitx5Utils REQUIRED)

# Setup some compiler option that is generally useful and compatible with Fcitx 5 (C++17)
include("${FCITX_INSTALL_CMAKECONFIG_DIR}/Fcitx5Utils/Fcitx5CompilerSettings.cmake")

include_directories(${CMAKE_SOURCE_DIR}/src)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fprofile-arcs -ftest-coverage -fno-access-control")
set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -fprofile-arcs -ftest-coverage -fno-access-control")

add_executable(runTests
    test.cpp
    ../src/animation/animationfactory.cpp
    ../src/animation/animationfactory.h
    ../src/animation/animator.h
    ../src/animation/disabledanimator.cpp
    ../src/animation/disabledanimator.h
    ../src/animation/enabledanimator.cpp
    ../src/animation/enabledanimator.h
    ../src/animation/expansionanimationfactory.cpp
    ../src/animation/expansionanimationfactory.h
    ../src/animation/floatanimationfactory.cpp
    ../src/animation/floatanimationfactory.h
    ../src/screenmanager.cpp
    ../src/virtualkeyboardsettings/virtualkeyboardsettings.cpp
    ../src/appinputareamanager.cpp
    ../src/ipc/fcitxvirtualkeyboardserviceproxy.cpp
    ../src/localsettings/viewlocalsettings.cpp
)

target_link_libraries(runTests
    Fcitx5::Core
    Fcitx5::Utils
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::Concurrent
    Qt6::DBus
    Qt6::Quick
    Qt6::QuickControls2
    KF6::WindowSystem
    Fcitx5Qt6::DBusAddons
    ${GSettings_QT_LIBRARIES}
)

add_test(NAME test COMMAND runTests)

install(TARGETS runTests DESTINATION ${CMAKE_INSTALL_BINDIR})