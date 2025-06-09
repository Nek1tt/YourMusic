cmake_minimum_required(VERSION 3.14)
project(ApiGateway)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Если используете vcpkg, при вызове cmake укажите toolchain:
#   -DCMAKE_TOOLCHAIN_FILE="C:/vcpkg/scripts/buildsystems/vcpkg.cmake"
# Тогда find_package найдёт Boost и nlohmann_json автоматически.

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/../../include/api
)

find_package(Boost REQUIRED COMPONENTS
    system
    thread
)

find_package(Threads REQUIRED)

find_package(nlohmann_json CONFIG REQUIRED)

add_executable(api-gateway
    api-gateway.cpp
)

target_link_libraries(api-gateway PRIVATE
    Boost::system
    Boost::thread
    nlohmann_json::nlohmann_json
    Threads::Threads
)

target_include_directories(api-gateway PRIVATE
    ${Boost_INCLUDE_DIRS}
)

set_target_properties(api-gateway PROPERTIES OUTPUT_NAME "api-gateway")
