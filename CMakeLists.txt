cmake_minimum_required(VERSION 3.5)

project(12 VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt6 REQUIRED COMPONENTS Multimedia)
#find_package(Qt5Multimedia REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        menu.ui
        menu.h
        menu.cpp
        resours.qrc
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(12
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET 12 APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(12 SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(12
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(12 PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(12 PRIVATE Qt6::Multimedia)
set_target_properties(12 PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

install(TARGETS 12
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

set(12_resource_files
    "images/diamond.png"
    "images/fon.png"
    "images/counter.png"
    "images/hint.png"
    "images/right.png"
    "images/wrong.png"
    "images/01.png"
    "images/02.png"
    "images/03.png"
    "images/04.png"
    "images/05.png"
    "images/06.png"
    "images/07.png"
    "images/melody.wav"
    "images/end.wav"
    "images/win.mp3"
    "images/wrong.wav"
    "images/right.wav"
    "images/on.png"
    "images/off.png"
    "images/icon.png"
)

qt_add_resources(12 "12"
    PREFIX
        "/"
    FILES
        ${12_resource_files}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(12)
endif()
