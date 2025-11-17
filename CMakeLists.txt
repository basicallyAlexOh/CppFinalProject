cmake_minimum_required(VERSION 3.16)

project(SimpleDAW VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPERIMENTAL_CXX_IMPORT_STD "")


# Find Qt6 (Widgets module)
find_package(Qt6 REQUIRED COMPONENTS Widgets)

include_directories("./include")


# Automatically run moc, rcc, uic
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

add_executable(SimpleDAW
    src/main.cpp
)

target_link_libraries(SimpleDAW PRIVATE Qt6::Widgets)
