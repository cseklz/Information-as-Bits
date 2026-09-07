cmake_minimum_required(VERSION 3.21)

project(Information_As_Bits VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

qt_add_executable(Information_As_Bits
    main.cpp
    MainWindow.cpp
    MainWindow.h
    HomePage.cpp
    HomePage.h
    AsciiWidget.cpp
    AsciiWidget.h
    AsciiConverter.cpp
    AsciiConverter.h
    BaseConverterWidget.cpp
    BaseConverterWidget.h
    BaseConverter.cpp
    BaseConverter.h
    PixelWidget.cpp
    PixelWidget.h
    PixelCodec.cpp
    PixelCodec.h
)

qt_add_resources(Information_As_Bits application_styles
    PREFIX "/style"
    BASE "style"
    FILES style/style.qss
)

target_link_libraries(Information_As_Bits PRIVATE Qt6::Widgets)

include(CTest)
if(BUILD_TESTING)
    add_executable(UnitTests
            tests/UnitTests.cpp)
    target_include_directories(UnitTests PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}")
    add_test(NAME UnitTests COMMAND UnitTests)
endif()
