cmake_minimum_required(VERSION 3.14)
project(StreamingService)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Если используете vcpkg, при вызове cmake укажите toolchain:
#   -DCMAKE_TOOLCHAIN_FILE="C:/vcpkg/scripts/buildsystems/vcpkg.cmake"
# Тогда find_package найдёт Boost, OpenSSL и nlohmann_json автоматически.

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/../../common
    ${CMAKE_CURRENT_SOURCE_DIR}/../../include/streaming
)

find_package(Boost REQUIRED COMPONENTS
    system
    thread
    charconv
)

find_package(OpenSSL REQUIRED)

find_package(Threads REQUIRED)

add_executable(streaming-service
    streaming-service.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../../common/db.cpp
)

target_link_libraries(streaming-service PRIVATE
    Boost::system
    Boost::thread
    Boost::charconv
    OpenSSL::SSL
    OpenSSL::Crypto
    Threads::Threads
)

target_include_directories(streaming-service PRIVATE
    ${Boost_INCLUDE_DIRS}
)

set_target_properties(streaming-service PROPERTIES OUTPUT_NAME "streaming-service")
