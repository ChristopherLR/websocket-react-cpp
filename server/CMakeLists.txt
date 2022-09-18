cmake_minimum_required(VERSION 3.24)

project(streaming VERSION 0.0.1)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(EXE_PATH "../bin")
# set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${EXE_PATH})
# set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${EXE_PATH})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${EXE_PATH})



include(CTest)

find_package(Catch2 3 REQUIRED)
find_package(websocketpp CONFIG REQUIRED)

enable_testing()

include_directories(src)
add_subdirectory(src)
