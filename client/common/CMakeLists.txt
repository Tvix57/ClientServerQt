add_library(common STATIC)

set_target_properties(common PROPERTIES
    AUTOMOC ON
    AUTOUIC ON
    AUTORCC ON
)

target_link_libraries(common
    Qt6::Core
    Qt6::Network
)

target_sources(common PRIVATE
    src/networkmanager.cpp
    src/protocols.cpp
    src/datagenerator.cpp
)

target_sources(common PRIVATE
    FILE_SET HEADERS
    BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/include
    FILES
        include/common/networkmanager.h
        include/common/protocols.h
        include/common/constants.h
        include/common/datagenerator.h
)

target_include_directories(common PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

if(WIN32)
    target_compile_definitions(common PRIVATE COMMON_LIBRARY)
endif()
