cmake_minimum_required(VERSION 4.3)
project(Information_As_Bits)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets )

add_executable(Information_As_Bits
        main.cpp
        MainWindow.h
        MainWindow.cpp
        Conversions.cpp
        Conversions.h)

qt_add_resources(Information_As_Bits "application_styles"
        PREFIX "/styles"
        BASE "styles"
        FILES
        styles/style.qss)

target_link_libraries(Information_As_Bits
        PRIVATE
            Qt::Core
            Qt::Gui
            Qt::Widgets
)