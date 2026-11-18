cmake_minimum_required(VERSION 3.20)

project(fps LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(glfw3 3.3 REQUIRED)

add_executable(fps
    main.cpp
    EngineLN.cpp
    /usr/local/lib/glad.c
)

target_include_directories(fps PRIVATE
    /urs/local/include/assimp/include
)

target_link_libraries(fps PRIVATE
    glfw
    "-framework OpenGL"
)

target_link_options(fps PRIVATE
    -fsanitize=address
)

target_compile_options(fps PRIVATE
    -w
    -g
    -O0
    -fsanitize=address
    -fno-omit-frame-pointer
)
