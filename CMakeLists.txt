cmake_minimum_required(VERSION 3.0)

project(test-sdl2)

find_package(SDL2 REQUIRED)
if(SDL2_FOUND)
    include_directories(${SDL2_INCLUDE_DIRS}/../)
    add_executable(
        main
        src/main.cpp
        src/init.c
        src/input.c
        src/draw.c
    )
    target_link_libraries(main ${SDL2_LIBRARIES})
endif()


# hard code
include_directories(
    /opt/homebrew/include
)
target_link_libraries(main -L/opt/homebrew/lib -lsdl2_image)

file(
    COPY
    ${CMAKE_CURRENT_SOURCE_DIR}/gfx
    DESTINATION
    ${CMAKE_BINARY_DIR})
