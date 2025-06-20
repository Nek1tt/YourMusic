cmake_minimum_required(VERSION 3.16)

project(musicproject VERSION 0.1 LANGUAGES CXX)

# Qt options
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# C++ Standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Указываем путь к pkg-config если нужно (Windows/MinGW)
# set(PKG_CONFIG_EXECUTABLE "C:/msys64/mingw64/bin/pkg-config")

# Найти Qt
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

# Список всех исходников
set(PROJECT_SOURCES
    main.cpp
    musicmain.cpp
    musicmain.h
    musicmain.ui

    albumwidget.cpp
    albumwidget.h

    createwidget.cpp
    createwidget.h

    profilewidget.cpp
    profilewidget.h

    rightbarwidget.cpp
    rightbarwidget.h

    myalbumswidget.cpp
    myalbumswidget.h

    mytrackswidget.cpp
    mytrackswidget.h

    userProfileWidget.cpp
    userProfileWidget.h
)

# Создание исполняемого файла
if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(musicproject
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
else()
    add_executable(musicproject
        ${PROJECT_SOURCES}
    )
endif()

# Линковка с Qt
target_link_libraries(musicproject PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

# Установка свойств (для macOS/iOS и Windows)
if(${QT_VERSION} VERSION_LESS 6.1.0)
    set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.musicproject)
endif()

set_target_properties(musicproject PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

# Установка
include(GNUInstallDirs)
install(TARGETS musicproject
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# Только для Qt6 — финализируем
if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(musicproject)
endif()
