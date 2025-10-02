cmake_minimum_required(VERSION 3.16)
project(Belady LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(Belady STATIC
    src/belady.hpp
)

set_target_properties(Belady PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(Belady PROPERTIES FOLDER "src")

target_include_directories(Belady
    PUBLIC src
)
