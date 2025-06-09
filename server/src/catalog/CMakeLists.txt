cmake_minimum_required(VERSION 3.14)
project(CatalogService)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/../../include/catalog   
    ${CMAKE_CURRENT_SOURCE_DIR}/../../common
)

# Если используете vcpkg, передавайте toolchain при вызове cmake:
# -DCMAKE_TOOLCHAIN_FILE="C:/vcpkg/scripts/buildsystems/vcpkg.cmake"
# Тогда Boost и nlohmann_json будут найдены автоматически.

find_package(Boost REQUIRED COMPONENTS
    system
    thread
    mysql
)
find_package(OpenSSL REQUIRED)
find_package(Threads REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)

add_executable(catalog-service
    catalog-service.cpp
    home-handler.cpp
    profile-handler.cpp
    create-handler.cpp
    search-handler.cpp
    search-user-handler.cpp
    user-action-handler.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../../common/db.cpp
)

target_link_libraries(catalog-service PRIVATE
    nlohmann_json::nlohmann_json
    Boost::system
    Boost::thread
    Boost::mysql
    OpenSSL::SSL
    OpenSSL::Crypto
    Threads::Threads
)

target_include_directories(catalog-service PRIVATE
    ${Boost_INCLUDE_DIRS}
)

set_target_properties(catalog-service PROPERTIES OUTPUT_NAME "catalog-service")
