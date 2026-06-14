cmake_minimum_required(VERSION 3.16)
project(qtsingleapplication)

find_package(Qt6 COMPONENTS Core Gui Widgets Network REQUIRED)

include_directories(src)
set(SRCS src/qtsingleapplication.h src/qtsingleapplication.cpp src/qtlocalpeer.h src/qtlocalpeer.cpp)

add_library(${PROJECT_NAME} STATIC ${SRCS})
target_include_directories(${PROJECT_NAME} PRIVATE src)
target_link_libraries(${PROJECT_NAME} PRIVATE
                                Qt6::Core
                                Qt6::Gui
                                Qt6::Widgets
                                Qt6::Network)
