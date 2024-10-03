cmake_minimum_required(VERSION 3.5.0)
project(exa_game_cpp VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)

add_executable(exa_game_cpp)

target_sources(exa_game_cpp
    PRIVATE
        ./main.cpp
        ./room.cpp
        ./scheduler.cpp
        ./room_entity.cpp
        ./floor.cpp
        ./cmd_debug.cpp
        ./bot.cpp
        ./file.cpp
        ./globals.cpp
)

