cmake_minimum_required(VERSION 3.5)

project(GBS LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets LinguistTools PrintSupport REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets LinguistTools PrintSupport REQUIRED)

include_directories("include/" "include/qcustomplot/")

set(TS_FILES translations/ru_RU.ts)

set(PROJECT_SOURCES
        src/main.cpp
        src/mainwindow.cpp
        src/mainwindow.h
        src/resources.qrc
        src/tablewidget.cpp
        src/tablewidget.h
        src/dialogs.cpp
        src/dialogs.h
        src/chartwidget.cpp
        src/chartwidget.h
        src/chartsettings.cpp
        src/chartsettings.h
        src/spacer.cpp
        src/spacer.h
        src/analyzediffractiondatadron2.cpp
        src/analyzediffractiondatadron2.h
        src/aboutprogram.cpp
        src/aboutprogram.h
        src/global.h
        ${TS_FILES}
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(GBS
        ${PROJECT_SOURCES}
    )

    qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
else()
    add_executable(GBS
        ${PROJECT_SOURCES}
    )

    qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
endif()


add_library(qcustomplot SHARED
    include/qcustomplot/qcustomplot.cpp
    include/qcustomplot/qcustomplot.h
)

target_link_libraries(qcustomplot PUBLIC Qt${QT_VERSION_MAJOR}::PrintSupport)
target_link_libraries(GBS PRIVATE Qt${QT_VERSION_MAJOR}::Widgets qcustomplot)
