cmake_minimum_required(VERSION 3.12)
project(multiscale_turing_pattern_cpp)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set(RESOURCE_FOLDER res)
set(RESOURCE_FILES ${RESOURCE_FOLDER}/resources.qrc)
set(PROJECT_SOURCE_DIR src/)

set(SOURCE_FILES
        src/main.cpp
        src/view/ScaleConfigWidget.cpp
        src/view/ScaleConfigWidget.h
        src/ScaleConfig.h
        src/Scale.h
        src/RenderConfig.h
        src/view/RenderConfigWidget.cpp
        src/view/RenderConfigWidget.h src/view/ScalesConfig.cpp src/view/ScalesConfig.h)

add_executable(${PROJECT_NAME} ${SOURCE_FILES} ${RESOURCE_FILES})

find_package(Qt5 REQUIRED COMPONENTS Core Widgets)
target_link_libraries(${PROJECT_NAME} Qt5::Core Qt5::Widgets)
