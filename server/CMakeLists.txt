cmake_minimum_required(VERSION 3.16)

project(server VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Gui Quick Qml)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Gui Quick Qml)

set(CPP_SOURCE
        ./main.cpp
        ./View/ServerWindow.cpp
        ./Model/Server.cpp
)

set(HPP_SOURCE
        ./View/ServerWindow.hpp
        ./Model/Server.hpp
)

set(UI_SOURCE
    UI/ServerWindow.qml
)

set(PROJECT_SOURCES
        ${CPP_SOURCE}
        ${HPP_SOURCE}
        ${UI_SOURCE}
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(server
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}   
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET server APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(server SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(server
            ${PROJECT_SOURCES}
        )
    endif()
endif()

qt_add_qml_module(server
    URI "server"
    VERSION 1.0
        QML_FILES UI/ServerWindow.qml
        QML_FILES UI/ClientDataRow.qml
        QML_FILES UI/ClientDataWindow.qml
)

target_link_libraries(server PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Quick
    Qt6::Qml
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.server)
endif()
set_target_properties(server PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS server
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(server)
endif()
