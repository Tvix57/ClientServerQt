# Исполнительный файл консольного клиента
add_executable(console_client)

# Установка свойств
set_target_properties(console_client PROPERTIES
    AUTOMOC ON
    AUTOUIC OFF
    AUTORCC OFF
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)

# Поиск Qt компонентов
find_package(Qt6 6.5.2 REQUIRED COMPONENTS Core Network)

# Исходные файлы
target_sources(console_client PRIVATE
    src/main.cpp
    src/consoleclient.cpp
)

target_sources(console_client PRIVATE
    FILE_SET HEADERS
    BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/include
    FILES
        include/console_client/consoleclient.h
)

# Заголовочные файлы
target_include_directories(console_client PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Подключение библиотек
target_link_libraries(console_client
    Qt6::Core
    Qt6::Network
    common  # Наша общая библиотека
)

# Установка
install(TARGETS console_client
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
