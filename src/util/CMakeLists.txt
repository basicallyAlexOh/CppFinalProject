file(GLOB UTIL_HEADERS
    ${CMAKE_SOURCE_DIR}/include/util/*.h
)

file(GLOB UTIL_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
)

add_library(UtilLayer
    ${UTIL_HEADERS}
    ${UTIL_SOURCES}
)
target_link_libraries(UtilLayer
    PUBLIC
        sndfile
        fftw3
)

target_include_directories(UtilLayer
    PUBLIC
        ${CMAKE_SOURCE_DIR}/include
)
