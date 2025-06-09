cmake_minimum_required(VERSION 3.10)
project(AudioPlayer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Поддержка потоков
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

# Пути к сторонним библиотекам
set(LIB_DIR "${CMAKE_SOURCE_DIR}/lib")

# FFmpeg
set(FFMPEG_DIR "${LIB_DIR}/ffmpeg-n7.1-latest-win64-gpl-shared-7.1")
set(FFMPEG_INCLUDE_DIR "${FFMPEG_DIR}/include")
set(FFMPEG_LIB_DIR "${FFMPEG_DIR}/lib")

# SDL2 (правильный путь)
set(SDL2_DIR "${LIB_DIR}/SDL2-2.32.8")
set(SDL2_INCLUDE_DIR "${SDL2_DIR}/include")
set(SDL2_LIB_DIR "${SDL2_DIR}/lib/x64")

# Boost
set(BOOST_DIR "${LIB_DIR}/boost_1_88_0")
set(BOOST_INCLUDE_DIR "${BOOST_DIR}")

# JSON (твои собственные хедеры)
set(PROJECT_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/include")



# Включаем директории
include_directories(
    ${FFMPEG_INCLUDE_DIR}
    ${SDL2_INCLUDE_DIR}
    ${BOOST_INCLUDE_DIR}
    ${PROJECT_INCLUDE_DIR}
)

# Линковочные директории
link_directories(
    ${FFMPEG_LIB_DIR}
    ${SDL2_LIB_DIR}
)

# Исполняемый файл
add_executable(player
    main.cpp
    AudioPlayer.cpp
)

# Линковка
target_link_libraries(player
    avformat
    avcodec
    SDL2main 
    avutil
    swresample
    SDL2       # здесь предполагается, что libSDL2.a или SDL2.dll лежит в SDL2_LIB_DIR
    ws2_32     # для Windows-сокетов
    Threads::Threads
)
