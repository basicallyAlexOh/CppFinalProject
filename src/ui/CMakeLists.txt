set(UI_HEADERS
    ${CMAKE_SOURCE_DIR}/include/ui/MainWindow.h
)

set(UI_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/MainWindow.cpp
)

add_library(UiLayer ${UI_HEADERS} ${UI_SOURCES})

target_include_directories(UiLayer PUBLIC
    ${CMAKE_SOURCE_DIR}/include
)
target_link_libraries(UiLayer PUBLIC Qt6::Widgets)
