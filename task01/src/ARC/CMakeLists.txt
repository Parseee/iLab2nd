add_library(ARC STATIC
    src/arc.cpp
    src/arc.hpp
)

set_target_properties(ARC PROPERTIES FOLDER "src")

target_include_directories(ARC
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include> # Для сборки проекта
        $<INSTALL_INTERFACE:include>                           # Для будущей установки
)