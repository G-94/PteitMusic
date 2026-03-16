cmake_minimum_required(VERSION 3.16)
project(PteitMusic VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# ========== ПУТИ ==========
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_PREFIX_PATH "D:/Programming/Msys2/mingw64")
set(QT_BIN_DIR "D:/Programming/Msys2/mingw64/bin")
set(QT_PLUGIN_DIR "D:/Programming/Msys2/mingw64/share/qt6/plugins")

find_package(Qt6 REQUIRED COMPONENTS
    Core
    Widgets
    Network
    Multimedia
    Concurrent
)

# ========== ВАШИ ФАЙЛЫ ==========
qt_add_executable(appPteitMusic
    main.cpp
    musicapi.cpp musicapi.h
    homepage.h homepage.cpp
    searchpagewidget.h searchpagewidget.cpp
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
    historyservice.h historyservice.cpp
    likesservice.h likesservice.cpp
    downloadsservice.h downloadsservice.cpp
    playlistsservice.h playlistsservice.cpp
    searchservice.h searchservice.cpp
    playerservice.h playerservice.cpp
)

target_include_directories(appPteitMusic PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(appPteitMusic PRIVATE
    Qt6::Core
    Qt6::Widgets
    Qt6::Gui
    Qt6::Network
    Qt6::Multimedia
    Qt6::Concurrent
)

set_target_properties(appPteitMusic PROPERTIES
    OUTPUT_NAME "PteitMusic"
    WIN32_EXECUTABLE FALSE
)

# ========== СОЗДАНИЕ СТРУКТУРЫ ПАПОК ==========
add_custom_command(TARGET appPteitMusic POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_SOURCE_DIR}/bin
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_SOURCE_DIR}/bin/platforms
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_SOURCE_DIR}/bin/styles
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_SOURCE_DIR}/bin/imageformats
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_SOURCE_DIR}/bin/multimedia
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_SOURCE_DIR}/Data
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_SOURCE_DIR}/Downloads
    COMMENT "Создание структуры папок"
)

# ========== КОПИРОВАНИЕ БАЗОВЫХ DLL ==========
set(NEEDED_DLLS
    # Qt ядро
    Qt6Core.dll
    Qt6Gui.dll
    Qt6Widgets.dll
    Qt6Network.dll
    Qt6Multimedia.dll
    Qt6Concurrent.dll

    # Компиляторные (обязательно)
    libgcc_s_seh-1.dll
    libstdc++-6.dll
    libwinpthread-1.dll

    # Графика и шрифты (нужны для интерфейса)
    libpng16-16.dll
    libjpeg-8.dll
    libfreetype-6.dll
    libharfbuzz-0.dll
    zlib1.dll
    libbrotlidec.dll
    libbrotlicommon.dll

    # Для сети (если используете HTTPS)
    libcrypto-3-x64.dll
    libssl-3-x64.dll

    # Для ICU (международная поддержка)
    libicudt78.dll
    libicuin78.dll
    libicuuc78.dll
)

foreach(DLL ${NEEDED_DLLS})
    if(EXISTS ${QT_BIN_DIR}/${DLL})
        add_custom_command(TARGET appPteitMusic POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                ${QT_BIN_DIR}/${DLL}
                ${CMAKE_SOURCE_DIR}/bin/${DLL}
            COMMENT "Копирование ${DLL}"
        )
    else()
        message(WARNING "⚠️ ${DLL} не найден в ${QT_BIN_DIR}")
    endif()
endforeach()

# ========== КОПИРОВАНИЕ ПЛАГИНОВ ==========

# 1. Platform plugin (обязательно для GUI)
if(EXISTS ${QT_PLUGIN_DIR}/platforms/qwindows.dll)
    add_custom_command(TARGET appPteitMusic POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${QT_PLUGIN_DIR}/platforms/qwindows.dll
            ${CMAKE_SOURCE_DIR}/bin/platforms/qwindows.dll
        COMMENT "Копирование platforms/qwindows.dll"
    )
else()
    message(FATAL_ERROR "❌ qwindows.dll не найден! Путь: ${QT_PLUGIN_DIR}/platforms/qwindows.dll")
endif()

# 2. Windows Media Foundation плагин (для аудио!)
if(EXISTS ${QT_PLUGIN_DIR}/multimedia/windowsmediaplugin.dll)
    add_custom_command(TARGET appPteitMusic POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${QT_PLUGIN_DIR}/multimedia/windowsmediaplugin.dll
            ${CMAKE_SOURCE_DIR}/bin/multimedia/windowsmediaplugin.dll
        COMMENT "✅ Копирование Windows Media плагина"
    )
elseif(EXISTS ${QT_PLUGIN_DIR}/multimedia/wmfmediaplugin.dll)
    # Альтернативное название
    add_custom_command(TARGET appPteitMusic POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${QT_PLUGIN_DIR}/multimedia/wmfmediaplugin.dll
            ${CMAKE_SOURCE_DIR}/bin/multimedia/wmfmediaplugin.dll
        COMMENT "✅ Копирование WMF Media плагина"
    )
else()
    message(WARNING "⚠️ Windows Media плагин не найден! Будет использован FFmpeg")
    # Если нет Windows, копируем FFmpeg как запасной вариант
    if(EXISTS ${QT_PLUGIN_DIR}/multimedia/ffmpegmediaplugin.dll)
        add_custom_command(TARGET appPteitMusic POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                ${QT_PLUGIN_DIR}/multimedia/ffmpegmediaplugin.dll
                ${CMAKE_SOURCE_DIR}/bin/multimedia/ffmpegmediaplugin.dll
            COMMENT "⚠️ Копирование FFmpeg плагина (запасной вариант)"
        )
    endif()
endif()

# 3. Image formats (для иконок и изображений)
foreach(IMG_PLUGIN jpeg gif ico svg)
    if(EXISTS ${QT_PLUGIN_DIR}/imageformats/q${IMG_PLUGIN}.dll)
        add_custom_command(TARGET appPteitMusic POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                ${QT_PLUGIN_DIR}/imageformats/q${IMG_PLUGIN}.dll
                ${CMAKE_SOURCE_DIR}/bin/imageformats/q${IMG_PLUGIN}.dll
            COMMENT "Копирование imageformats/q${IMG_PLUGIN}.dll"
        )
    endif()
endforeach()

# 4. Style (опционально, для внешнего вида)
if(EXISTS ${QT_PLUGIN_DIR}/styles/qmodernwindowsstyle.dll)
    add_custom_command(TARGET appPteitMusic POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${QT_PLUGIN_DIR}/styles/qmodernwindowsstyle.dll
            ${CMAKE_SOURCE_DIR}/bin/styles/qmodernwindowsstyle.dll
        COMMENT "Копирование styles/qmodernwindowsstyle.dll"
    )
endif()

# ========== УДАЛЯЕМ FFMPEG DLL (чтобы не было конфликтов) ==========
add_custom_command(TARGET appPteitMusic POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E remove -f
        ${CMAKE_SOURCE_DIR}/bin/avcodec-*.dll
        ${CMAKE_SOURCE_DIR}/bin/avformat-*.dll
        ${CMAKE_SOURCE_DIR}/bin/avutil-*.dll
        ${CMAKE_SOURCE_DIR}/bin/sw*.dll
        ${CMAKE_SOURCE_DIR}/bin/libmp3*.dll
        ${CMAKE_SOURCE_DIR}/bin/libx*.dll
    COMMENT "🧹 Удаление FFmpeg DLL (не нужны для Windows плагина)"
)

# ========== СОЗДАНИЕ qt.conf ==========
file(WRITE ${CMAKE_SOURCE_DIR}/bin/qt.conf
"[Paths]
Plugins = .
")

# ========== ФИНАЛЬНАЯ ПРОВЕРКА ==========
add_custom_command(TARGET appPteitMusic POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E echo "✅ Сборка завершена!"
    COMMAND ${CMAKE_COMMAND} -E echo "📁 Проверьте папку bin/multimedia/ - там должен быть windowsmediaplugin.dll"
    COMMAND ${CMAKE_COMMAND} -E echo "📁 FFmpeg DLL были удалены (они не нужны)"
)
