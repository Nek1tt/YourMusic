# server/src/api-gateway/CMakeLists.txt

cmake_minimum_required(VERSION 3.14)
project(ApiGateway)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Если используете vcpkg, при вызове cmake укажите toolchain:
#   -DCMAKE_TOOLCHAIN_FILE="C:/vcpkg/scripts/buildsystems/vcpkg.cmake"
# Тогда find_package найдёт Boost и nlohmann_json автоматически.

# Пути к заголовочным файлам (относительно server/src/api-gateway)
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/../../include/api        # для api-gateway.h
)

# Ищем Boost с компонентами system и thread (для Asio/Beast)
find_package(Boost REQUIRED COMPONENTS
    system
    thread
)

# Ищем Threads (стандартные потоки)
find_package(Threads REQUIRED)

# Ищем nlohmann_json (header-only библиотека для JSON)
find_package(nlohmann_json CONFIG REQUIRED)

# Собираем исполняемый файл из одного .cpp
add_executable(api-gateway
    api-gateway.cpp
)

# Линкуем с Boost::system, Boost::thread, nlohmann_json и потоками
target_link_libraries(api-gateway PRIVATE
    Boost::system
    Boost::thread
    nlohmann_json::nlohmann_json
    Threads::Threads
)

# Указываем include-дира Boost (header-only модули найдутся автоматически)
target_include_directories(api-gateway PRIVATE
    ${Boost_INCLUDE_DIRS}
)

# Задаём конечное имя бинарника
set_target_properties(api-gateway PROPERTIES OUTPUT_NAME "api-gateway")