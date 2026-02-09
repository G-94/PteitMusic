cmake_minimum_required(VERSION 3.16)
project(PteitMusic VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Добавили Core компонент
find_package(Qt6 REQUIRED COMPONENTS
    Core        # <--- ДОБАВИЛИ ЭТО
    Widgets
    Multimedia
    MultimediaWidgets
    Network
)

# Создаем исполняемый файл
qt_add_executable(appPteitMusic
    main.cpp
    musicapi.cpp
    musicapi.h
    homepage.h homepage.cpp
    searchpanelwidget.h
    playerwidget.h playerwidget.cpp
    searchpanelwidget.cpp
    songtracklistitem.h songtracklistitem.cpp
    scrollabletracklist.h scrollabletracklist.cpp
    likespagewidget.h likespagewidget.cpp
)

# Подключаем заголовочные файлы
target_include_directories(appPteitMusic PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

# Подключаем библиотеки (добавили Qt6::Core)
target_link_libraries(appPteitMusic
    PRIVATE
        Qt6::Core       # <--- ДОБАВИЛИ ЭТО
        Qt6::Widgets
        Qt6::Multimedia
        Qt6::MultimediaWidgets
        Qt6::Network
)

# Windows библиотеки
if(WIN32)
    target_link_libraries(appPteitMusic
        PRIVATE
            ws2_32
            user32
            gdi32
            winmm
            advapi32
    )

    if(MINGW)
        add_compile_definitions(_WIN32_WINNT=0x0601)
    endif()

    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -O0")
    endif()
endif()
