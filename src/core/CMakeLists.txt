file(GLOB CORE_HEADERS
    ${CMAKE_SOURCE_DIR}/include/core/*.h
)

file(GLOB CORE_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
)

add_library(CoreLayer
    ${CORE_HEADERS}
    ${CORE_SOURCES}
)
target_link_libraries(CoreLayer
    PUBLIC
        sndfile
)

target_include_directories(CoreLayer
    PUBLIC
        ${CMAKE_SOURCE_DIR}/include
)
