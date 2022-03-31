cmake_minimum_required(VERSION 3.0)
project(Checkers)

set(CMAKE_CXX_STANDARD 20)

set(SOURCES
        source/ABSearch.hpp source/ABSearch.cpp
        source/Game.hpp source/Game.cpp
        source/Table.hpp source/Table.cpp
        source/Vector.hpp source/Vector.cpp)

add_executable(${PROJECT_NAME} play/main.cpp ${SOURCES})
