cmake_minimum_required(VERSION 3.14)
project(AuthService)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Если вы используете vcpkg, достаточно при вызове cmake указать toolchain:
#   -DCMAKE_TOOLCHAIN_FILE="C:/vcpkg/scripts/buildsystems/vcpkg.cmake"
# Тогда CMake автоматически найдёт Boost.

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/../../include/auth
    ${CMAKE_CURRENT_SOURCE_DIR}/../../common
)

find_package(Boost REQUIRED COMPONENTS
    system
    thread
    mysql
)

find_package(OpenSSL REQUIRED)
find_package(Threads REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)

add_executable(auth-service
    auth-service.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../../common/db.cpp
)

target_link_libraries(auth-service PRIVATE
    nlohmann_json::nlohmann_json
    Boost::system
    Boost::thread
    Boost::mysql
    OpenSSL::SSL
    OpenSSL::Crypto
    Threads::Threads
)

target_include_directories(auth-service PRIVATE
    ${Boost_INCLUDE_DIRS}
)

set_target_properties(auth-service PROPERTIES OUTPUT_NAME "auth-service")
