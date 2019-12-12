
cmake_minimum_required(VERSION 3.10)

set(PROJECT_NAME CellCLA)
project(${PROJECT_NAME} VERSION 0.1.0 LANGUAGES CXX)

# Produces a target 'cellml'
find_package(libCellML REQUIRED)

add_subdirectory(src)

