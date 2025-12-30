cmake_minimum_required(VERSION 3.16)

project(musicproject VERSION 0.1 LANGUAGES CXX)

# --- 1. Включаем автоматизацию Qt (Самое важное!) ---
# Это избавляет от необходимости писать qt6_wrap_cpp и вручную обрабатывать .ui файлы
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# --- 2. Стандарты C++ ---
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# --- 3. Находим пакеты Qt 6 ---
# Мы ищем сразу Widgets и WebSockets. Network обычно нужен для WebSockets.
find_package(Qt6 REQUIRED COMPONENTS Widgets WebSockets Network)

# --- 4. Список всех файлов проекта ---
# Не нужно разделять хедеры и cpp, и не нужно писать их дважды.
set(PROJECT_SOURCES
    main.cpp
    musicmain.cpp      musicmain.h      musicmain.ui
    albumwidget.cpp    albumwidget.h
    createTab.cpp      # createTab.h (раскомментируйте, если файл существует)
    profiletab.cpp     # profiletab.h
    rightbarwidget.cpp rightbarwidget.h
    myalbumswidget.cpp myalbumswidget.h
    mytrackswidget.cpp mytrackswidget.h
    userProfileWidget.cpp userProfileWidget.h
    loginwindow.cpp    loginwindow.h
    albomlistwidget.cpp albomlistwidget.h
    userslistwidget.cpp userslistwidget.h
    profilewidget.cpp  profilewidget.h
    websocketclient.cpp websocketclient.h
    hometab.cpp        hometab.h
    homewidget.cpp     homewidget.h
    createwidget.cpp   createwidget.h
    setstyle.cpp       setstyle.h
)

# --- 5. Создание исполняемого файла ---
qt_add_executable(musicproject
    MANUAL_FINALIZATION
    ${PROJECT_SOURCES}
    # Если у вас есть файл ресурсов .qrc, добавьте его сюда
)

# --- 6. Линковка библиотек ---
target_link_libraries(musicproject PRIVATE 
    Qt6::Widgets 
    Qt6::WebSockets
    Qt6::Network
)

# --- 7. Копирование JSON файлов (Опционально) ---
# Чтобы программа видела JSON файлы при запуске, их нужно скопировать в папку сборки
# Создайте папку resources/jsons рядом с исходниками, если еще нет.
file(COPY resources/jsons DESTINATION ${CMAKE_BINARY_DIR}/resources)

# --- 8. Настройки для Windows/macOS ---
if(WIN32)
    # Убирает консольное окно при запуске (для релизной версии)
    set_target_properties(musicproject PROPERTIES WIN32_EXECUTABLE TRUE)
endif()

if(APPLE)
    set_target_properties(musicproject PROPERTIES
        MACOSX_BUNDLE TRUE
        MACOSX_BUNDLE_GUI_IDENTIFIER com.example.musicproject
        MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
        MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    )
endif()

# --- 9. Финализация (для Qt 6) ---
qt_finalize_executable(musicproject)