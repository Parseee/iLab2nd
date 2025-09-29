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
add_subdirectory(src/Belady)
include_directories(src)

set(ARC_SOURCES 
    src/arc.cpp
    )
    
add_executable(arc-cache ${ARC_SOURCES})

target_link_libraries(
    arc-cache PRIVATE project_compiler_options ARC)

    # keep project_compiler_options interface as you had it
target_compile_options(project_compiler_options INTERFACE
  $<$<CONFIG:Debug>:-Wall -Wextra -g -fno-omit-frame-pointer -fsanitize=address,leak>
  $<$<CONFIG:Release>:-O2>
)

# link-time sanitizer flags (if needed) should go to link options for Debug
target_link_options(arc-cache PRIVATE
  $<$<CONFIG:Debug>:-fsanitize=address,leak>
)

set(BELADY_SOURCES 
    src/belady.cpp
    )
    
add_executable(belady-cache ${BELADY_SOURCES})

target_link_libraries(
    belady-cache PRIVATE project_compiler_options Belady)

    # keep project_compiler_options interface as you had it
target_compile_options(project_compiler_options INTERFACE
  $<$<CONFIG:Debug>:-Wall -Wextra -g -fno-omit-frame-pointer -fsanitize=address,leak>
  $<$<CONFIG:Release>:-O2>
)

# link-time sanitizer flags (if needed) should go to link options for Debug
target_link_options(belady-cache PRIVATE
  $<$<CONFIG:Debug>:-fsanitize=address,leak>
)