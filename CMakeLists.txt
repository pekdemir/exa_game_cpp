cmake_minimum_required(VERSION 3.5.0)
project(exa_game_cpp VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)


file(GLOB exa_game_cpp_src
    "*.cpp"
)

add_executable(exa_game_cpp ${exa_game_cpp_src})

