cmake_minimum_required(VERSION 4.0)
set(CMAKE_EXPERIMENTAL_CXX_IMPORT_STD "d0edc3af-4c50-42ea-a356-e2862fe7a444")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


project(SimpleDAW VERSION 0.1.0 LANGUAGES C CXX)

# Find Qt6 (Widgets module)
find_package(Qt6 REQUIRED COMPONENTS Core Multimedia Widgets)
qt_standard_project_setup()


# Set the C++ standard to 23 (or CMAKE_CXX_STANDARD_LATEST)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)


include_directories("./include")


# Automatically run moc, rcc, uic
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)


add_library(Core STATIC)
add_subdirectory(./src)


add_executable(SimpleDAW src/main.cpp)
add_dependencies(SimpleDAW Core)

target_link_libraries(SimpleDAW PRIVATE
    Qt6::Core
    Qt6::Multimedia
    Qt6::Widgets
)
