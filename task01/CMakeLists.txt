cmake_minimum_required(VERSION 3.10)
project(arc-cache CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(project_compiler_options INTERFACE)
target_compile_options(project_compiler_options INTERFACE
    $<$<CONFIG:Debug>:-Wall -Wextra -g -fno-omit-frame-pointer -fsanitize=address,leak>
    $<$<CONFIG:Release>:-O2>
)

add_subdirectory(src/ARC)
include_directories(src)

set(SOURCES 
    src/main.cpp
    )
    
add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(
    ${PROJECT_NAME} PRIVATE project_compiler_options ARC)

    # keep project_compiler_options interface as you had it
target_compile_options(project_compiler_options INTERFACE
  $<$<CONFIG:Debug>:-Wall -Wextra -g -fno-omit-frame-pointer -fsanitize=address,leak>
  $<$<CONFIG:Release>:-O2>
)

# link-time sanitizer flags (if needed) should go to link options for Debug
target_link_options(${PROJECT_NAME} PRIVATE
  $<$<CONFIG:Debug>:-fsanitize=address,leak>
)