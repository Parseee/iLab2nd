add_library(Belady STATIC
    src/belady.hpp
)

set_target_properties(Belady PROPERTIES LINKER_LANGUAGE CXX)

set(CMAKE_CXX_STANDART 20)


set_target_properties(Belady PROPERTIES FOLDER "src")

target_include_directories(ARC
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include> # Для сборки проекта
        $<INSTALL_INTERFACE:include>                           # Для будущей установки
)