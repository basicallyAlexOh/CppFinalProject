# Build the three libraries
add_subdirectory(core)
add_subdirectory(util)
add_subdirectory(ui)

# Main executable
add_executable(MyQtApp
    main.cpp
)

# Link against UI; UI will pull in Core/Util and Qt via PUBLIC deps
target_link_libraries(MyQtApp
    PRIVATE
        UiLayer
)
