cmake_minimum_required(VERSION 3.20)
project(client VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Поиск Qt
find_package(Qt6 6.5.2 REQUIRED COMPONENTS Core Network)

# Общая библиотека
add_subdirectory(common)

# GUI клиент
# option(BUILD_GUI_CLIENT "Build GUI client" OFF)
# if(BUILD_GUI_CLIENT)
#     find_package(Qt6 6.5.2 REQUIRED COMPONENTS Quick Qml)
#     add_subdirectory(gui_client)
# endif()

# Консольный клиент
add_subdirectory(console_client)

include(GNUInstallDirs)
install(TARGETS console_client
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
