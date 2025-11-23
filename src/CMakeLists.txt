# Add submodules
add_subdirectory(core)
add_subdirectory(ui)
add_subdirectory(util)

# Collect main.cpp
set(SRC_MAIN
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
)

# Create the executable
qt_add_executable(MyQtApp
    ${SRC_MAIN}
)

# Link against ui/ and util/ libs
target_link_libraries(MyQtApp PRIVATE
    UiLayer
    UtilLayer
    Qt6::Widgets
)

target_include_directories(MyQtApp PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)