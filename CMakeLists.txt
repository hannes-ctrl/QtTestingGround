cmake_minimum_required(VERSION 3.16)

project(ZNI-BG_Testing VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 REQUIRED COMPONENTS Core Gui Widgets)

set(PROJECT_SOURCES
        src/main.cpp

        src/gui/mainwindow.cpp
        src/gui/mainwindow.h
        src/gui/mainwindow.ui

        src/core/testCase.cpp
        src/core/testCase.h
        src/core/testResult.cpp
        src/core/testResult.h
        src/core/testContext.cpp
        src/core/testContext.h
        src/core/testSession.cpp
        src/core/testSession.h
        src/core/testRunner.cpp
        src/core/testRunner.h

        src/tests/zlvTest.cpp
        src/tests/zlvTest.h
        src/tests/ttyTest.cpp
        src/tests/ttyTest.h
        src/tests/switchCardsTest.cpp
        src/tests/switchCardsTest.h

        src/hardware/hardwareInterface.h
        src/hardware/zlvBusInterface.h
        src/hardware/ttyInterface.h
        src/hardware/switchCardInterface.h
        src/hardware/simulatedZlvBus.cpp
        src/hardware/simulatedZlvBus.h
        src/hardware/simulatedTty.cpp
        src/hardware/simulatedTty.h
        src/hardware/simulatedSwitchCard.cpp
        src/hardware/simulatedSwitchCard.h

        src/report/reportWriter.cpp
        src/report/reportWriter.h

        src/config/config.cpp
        src/config/config.h

        src/util/logger.cpp
        src/util/logger.h
)

add_executable(ZNI-BG_Testing ${PROJECT_SOURCES})

target_include_directories(ZNI-BG_Testing PRIVATE src)

target_link_libraries(ZNI-BG_Testing PRIVATE
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets
)

set_target_properties(ZNI-BG_Testing PROPERTIES
        WIN32_EXECUTABLE TRUE
)
