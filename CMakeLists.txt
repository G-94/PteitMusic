cmake_minimum_required(VERSION 3.16)
project(PteitMusic VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# ========== НАСТРОЙКА ВЫХОДНОЙ ПАПКИ ==========
# ВСЕ файлы будут в папке bin в корне проекта
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

# ========== ПОИСК QT ==========
find_package(Qt6 REQUIRED COMPONENTS
    Core
    Widgets
    Multimedia
    MultimediaWidgets
    Network
    Concurrent
)

# ========== ПОЛУЧАЕМ ПУТИ К QT ==========
get_target_property(QT_CORE_LOCATION Qt6::Core LOCATION)
get_filename_component(QT_BIN_DIR ${QT_CORE_LOCATION} DIRECTORY)
get_filename_component(QT_ROOT_DIR ${QT_BIN_DIR} DIRECTORY)

message(STATUS "Qt binaries: ${QT_BIN_DIR}")

# ========== СОЗДАНИЕ ИСПОЛНЯЕМОГО ФАЙЛА ==========
qt_add_executable(appPteitMusic
    main.cpp
    musicapi.cpp musicapi.h
    homepage.h homepage.cpp
    searchpanelwidget.h searchpanelwidget.cpp
    playerwidget.h playerwidget.cpp
    songtracklistitem.h songtracklistitem.cpp
    scrollabletracklist.h scrollabletracklist.cpp
    likespagewidget.h likespagewidget.cpp
    downloadpagewidget.h downloadpagewidget.cpp
    MusicGlobals.h
    playlistshubpagewidget.h playlistshubpagewidget.cpp
    genrelistwidget.h genrelistwidget.cpp
    artistslistwidget.h artistslistwidget.cpp
    artistslistitem.h artistslistitem.cpp
    styles.h styles.cpp


)

target_include_directories(appPteitMusic PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(appPteitMusic
    PRIVATE
        Qt6::Core
        Qt6::Widgets
        Qt6::Multimedia
        Qt6::MultimediaWidgets
        Qt6::Network
        Qt6::Concurrent
)

# ========== Windows библиотеки ==========
if(WIN32)
    target_link_libraries(appPteitMusic PRIVATE ws2_32 user32 gdi32 winmm advapi32)
endif()

set_target_properties(appPteitMusic PROPERTIES
    OUTPUT_NAME "PteitMusic"
    WIN32_EXECUTABLE TRUE
)

# ========== АВТОМАТИЧЕСКОЕ КОПИРОВАНИЕ ВСЕХ НУЖНЫХ DLL ==========
if(WIN32)
    # Создаем необходимые папки
    file(MAKE_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")
    file(MAKE_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/platforms")
    file(MAKE_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/Data")

    # 1. КОПИРУЕМ ВСЕ ОСНОВНЫЕ QT DLL
    set(QT_DLLS
        Qt6Core.dll
        Qt6Gui.dll
        Qt6Widgets.dll
        Qt6Multimedia.dll
        Qt6MultimediaWidgets.dll
        Qt6Network.dll
        Qt6Concurrent.dll
    )

    foreach(DLL ${QT_DLLS})
        set(SOURCE_DLL "${QT_BIN_DIR}/${DLL}")
        if(EXISTS ${SOURCE_DLL})
            add_custom_command(TARGET appPteitMusic POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                    "${SOURCE_DLL}"
                    "${CMAKE_SOURCE_DIR}/bin/"
                COMMENT "Копирование ${DLL}..."
            )
        endif()
    endforeach()

    # 2. КОПИРУЕМ MINGW DLL (обязательно!)
    set(MINGW_DLLS
        libgcc_s_seh-1.dll
        libstdc++-6.dll
        libwinpthread-1.dll
    )

    foreach(DLL ${MINGW_DLLS})
        set(SOURCE_DLL "${QT_BIN_DIR}/${DLL}")
        if(EXISTS ${SOURCE_DLL})
            add_custom_command(TARGET appPteitMusic POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                    "${SOURCE_DLL}"
                    "${CMAKE_SOURCE_DIR}/bin/"
                COMMENT "Копирование ${DLL}..."
            )
        endif()
    endforeach()

    # 3. КОПИРУЕМ ПЛАТФОРМЕННЫЙ ПЛАГИН (ОБЯЗАТЕЛЬНО!)
    set(PLATFORM_PLUGIN "${QT_ROOT_DIR}/plugins/platforms/qwindows.dll")
    if(NOT EXISTS ${PLATFORM_PLUGIN})
        set(PLATFORM_PLUGIN "${QT_BIN_DIR}/plugins/platforms/qwindows.dll")
    endif()

    if(EXISTS ${PLATFORM_PLUGIN})
        add_custom_command(TARGET appPteitMusic POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                "${PLATFORM_PLUGIN}"
                "${CMAKE_SOURCE_DIR}/bin/platforms/qwindows.dll"
            COMMENT "✅ Копирование qwindows.dll (платформенный плагин)"
        )
    else()
        message(WARNING "⚠️ qwindows.dll не найден! Приложение может не запуститься.")
    endif()

    # 4. ПРОВЕРКА РЕЗУЛЬТАТА
    add_custom_command(TARGET appPteitMusic POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E echo ""
        COMMAND ${CMAKE_COMMAND} -E echo "========================================="
        COMMAND ${CMAKE_COMMAND} -E echo "✅ СБОРКА ЗАВЕРШЕНА!"
        COMMAND ${CMAKE_COMMAND} -E echo "📁 Исполняемый файл: ${CMAKE_SOURCE_DIR}/bin/PteitMusic.exe"
        COMMAND ${CMAKE_COMMAND} -E echo "📁 Папка platforms: ${CMAKE_SOURCE_DIR}/bin/platforms/"
        COMMAND ${CMAKE_COMMAND} -E echo "========================================="
    )
endif()
