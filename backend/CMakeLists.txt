cmake_minimum_required(VERSION 3.10)
cmake_policy(SET CMP0115 NEW)

project(AudioPlayer)

set(CMAKE_CXX_STANDARD 17)

find_package(PkgConfig REQUIRED)
pkg_check_modules(FFMPEG REQUIRED libavformat libavcodec libavutil libswresample sdl2)

include_directories(
    ${FFMPEG_INCLUDE_DIRS}
    include 
)

link_directories(${FFMPEG_LIBRARY_DIRS})

add_executable(player main.cpp AudioPlayer.cpp)

target_link_libraries(player ${FFMPEG_LIBRARIES} SDL2)
