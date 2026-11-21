cmake_minimum_required(VERSION 3.20)
project(fps LANGUAGES C CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(glfw3 3.3 REQUIRED)
find_package(PkgConfig REQUIRED)

add_executable(fps
    main.cpp
    EngineLN.cpp
    /usr/local/lib/glad.c
)

target_compile_features(fps PRIVATE cxx_std_20)

target_include_directories(fps PRIVATE
    /opt/homebrew/include
    /usr/local/include
)

target_link_directories(fps PRIVATE
    /opt/homebrew/lib
    /usr/local/lib
)

target_link_libraries(fps PRIVATE
    glfw
    "-framework OpenGL"
    enet
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
