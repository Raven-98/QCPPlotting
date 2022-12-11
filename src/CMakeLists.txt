cmake_minimum_required(VERSION 3.5)

project(QCPPlotting VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets PrintSupport)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets PrintSupport)

include_directories("qcustomplot/")

set(PROJECT_SOURCES
        main.cpp
        mainwindow.h mainwindow.cpp
        resources.qrc
        structs.h
        consts.h
        tablewidget.h tablewidget.cpp
        chartwidget.h chartwidget.cpp
        logicalmethods.h logicalmethods.cpp
        dialogs.h dialogs.cpp
        separatorwidgets.h separatorwidgets.cpp
        pushbuttoncolorpicker.h pushbuttoncolorpicker.cpp
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(QCPPlotting
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
    qt_add_library(qcustomplot MODULE
        MANUAL_FINALIZATION
        qcustomplot/qcustomplot.cpp
        qcustomplot/qcustomplot.h
    )
else()
    add_executable(QCPPlotting
        ${PROJECT_SOURCES}
    )
#    add_library(qcustomplot STATIC
    add_library(qcustomplot SHARED
#    add_library(qcustomplot MODULE
        qcustomplot/qcustomplot.cpp
        qcustomplot/qcustomplot.h
    )
endif()

target_link_libraries(qcustomplot PUBLIC Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::PrintSupport)
target_link_libraries(QCPPlotting PRIVATE Qt${QT_VERSION_MAJOR}::Widgets qcustomplot)

set_target_properties(QCPPlotting PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

install(TARGETS QCPPlotting
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(QCPPlotting)
endif()
