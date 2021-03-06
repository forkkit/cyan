#
# Cyan <https://cyan.fxarena.net>
# Copyright Ole-André Rodlie, FxArena DA.
#
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 2.1 of the License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.
#

cmake_minimum_required(VERSION 3.0)

project(Cyan VERSION 1.99.3)
if(NOT PROJECT_VERSION_TWEAK)
    set(PROJECT_VERSION_TWEAK 0)
endif()

if(DEFINED ENV{CYAN_VERSION})
    set(PROJECT_VERSION $ENV{CYAN_VERSION})
endif()
if(DEFINED ENV{CYAN_GIT})
    set(PROJECT_VERSION_GIT $ENV{CYAN_GIT})
endif()

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 11)
set(COMPANY "FxArena DA")
set(COPYRIGHT "Copyright Ole-Andre Rodlie, FxArena DA. All rights reserved.")
set(IDENTIFIER "net.fxarena.cyan")
set(WEBSITE "https://cyan.fxarena.net")
set(PROJECT_VERSION_TYPE "Community" CACHE STRING "Version type")
set(INSTALLER_ID "0830D6C5-BB35-41E6-B694-971AA1CC8059" CACHE STRING "Windows Installer ID")
set(MAGICK_PKG_CONFIG "Magick++" CACHE STRING "ImageMagick pkg-config name")
option(BUILD_LIBRARY "Build shared CyanApi library" OFF)
option(BUILD_WIDGET_PLUGIN_EXAMPLE "Build widget plugin example" OFF)
option(BUILD_WIDGET_PLUGIN_COLOR_TRIANGLE "Build color triangle plugin" OFF)
option(ENABLE_FFMPEG "FFmpeg support (if available)" ON)
option(DEBUG_TILES "Show tiles for debug purpose" OFF)
option(LINUX_DEPLOY "Linux deploy" OFF)
option(ENABLE_FONTCONFIG "Fontconfig support on deployment" OFF)
option(MINGW32 "mingw32" OFF)

if(${LINUX_DEPLOY})
    set(CMAKE_INSTALL_RPATH "$ORIGIN/../lib")
    find_package(Threads REQUIRED)
endif()

set(
    HEADERS_API
    lib/CyanWidgetApi.h)
set(
    SOURCES_LIB
    lib/CyanAboutDialog.cpp
    lib/CyanImageFormat.cpp
    lib/CyanTileItem.cpp
    lib/CyanLayerItem.cpp
    lib/CyanLayerItemDelegate.cpp
    lib/CyanView.cpp
    lib/CyanColorConvert.cpp
    lib/CyanColorConvertDialog.cpp
    lib/CyanPluginLoader.cpp
    lib/CyanMdi.cpp
    lib/CyanMdiWindow.cpp
    lib/CyanLayerWidget.cpp
    lib/CyanLayerTree.cpp
    lib/CyanLayerTreeItem.cpp
    #lib/CyanTextWidget.cpp
    #lib/CyanSliderCMYK.cpp
    #lib/CyanSliderHSV.cpp
    #lib/CyanSliderRGB.cpp
    lib/CyanFFmpeg.cpp
    lib/CyanFFmpegOpenDialog.cpp
    lib/CyanNewMediaDialog.cpp
    #lib/CyanRuler.cpp
    lib/CyanGuideItem.cpp
    lib/qtcolorpicker.cpp
    lib/qtcolortriangle.cpp
    lib/qtwindowlistmenu.cpp)
set(
    HEADERS_LIB
    lib/CyanAboutDialog.h
    lib/CyanImageFormat.h
    lib/CyanTileItem.h
    lib/CyanLayerItem.h
    lib/CyanLayerItemDelegate.h
    lib/CyanView.h
    lib/CyanColorConvert.h
    lib/CyanColorConvertDialog.h
    lib/CyanPluginLoader.h
    lib/CyanMdi.h
    lib/CyanMdiWindow.h
    lib/CyanLayerWidget.h
    lib/CyanLayerTree.h
    lib/CyanLayerTreeItem.h
    #lib/CyanTextWidget.h
    #lib/CyanSliderCMYK.h
    #lib/CyanSliderHSV.h
    #lib/CyanSliderRGB.h
    lib/CyanFFmpeg.h
    lib/CyanFFmpegOpenDialog.h
    lib/CyanNewMediaDialog.h
    #lib/CyanRuler.h
    lib/CyanGuideItem.h
    lib/CyanGlobal.h
    lib/qtcolorpicker.h
    lib/qtcolortriangle.h
    lib/qtwindowlistmenu.h)
set(
    SOURCES_APP
    app/cyan_common.cpp
    #app/widgets/cyan_textwidget.cpp
    #app/widgets/colorrgb.cpp
    #app/widgets/colorcmyk.cpp
    #app/widgets/colorhsv.cpp
    #app/dialogs/newmediadialog.cpp
    #app/dialogs/convertdialog.cpp
    app/main.cpp
    #app/converter.cpp
    app/editor.cpp
    #app/about.cpp
    app/setup.cpp
    app/color.cpp
    app/viewtool.cpp
    app/messages.cpp
    app/layers.cpp
    app/tabs.cpp)
set(
    HEADERS_APP
    app/cyan_common.h
    #app/widgets/cyan_textwidget.h
    #app/widgets/colorrgb.h
    #app/widgets/colorcmyk.h
    #app/widgets/colorhsv.h
    #app/dialogs/newmediadialog.h
    #app/dialogs/convertdialog.h
    #app/converter.h
    app/editor.h)
set(RESOURCES share/icons.qrc docs/docs.qrc)

add_definitions(-DCYAN_VERSION="${PROJECT_VERSION}")
add_definitions(-DCYAN_IMAGE_FORMAT_VERSION="${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}")
add_definitions(-DCYAN_VERSION_MAJOR="${PROJECT_VERSION_MAJOR}")
add_definitions(-DCYAN_VERSION_MINOR="${PROJECT_VERSION_MINOR}")
add_definitions(-DCYAN_VERSION_PATCH="${PROJECT_VERSION_PATCH}")
add_definitions(-DCYAN_VERSION_GIT="${PROJECT_VERSION_GIT}")
add_definitions(-DCYAN_VERSION_TYPE="${PROJECT_VERSION_TYPE}")
add_definitions(-DCYAN_WEBSITE="${WEBSITE}")
add_definitions(-DCYAN_LIBRARY)

find_package(Qt5Core REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Concurrent REQUIRED)
find_package(Qt5Test REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_search_module(MAGICK REQUIRED ${MAGICK_PKG_CONFIG})
pkg_search_module(LCMS2 REQUIRED lcms2)
#pkg_search_module(LUA REQUIRED lua)
if(${ENABLE_FFMPEG})
    pkg_search_module(AVUTIL OPTIONAL libavutil)
    pkg_search_module(AVDEVICE OPTIONAL libavdevice)
    pkg_search_module(SWSCALE OPTIONAL libswscale)
    pkg_search_module(AVFORMAT OPTIONAL libavformat)
    pkg_search_module(AVCODEC OPTIONAL libavcodec)
endif()

if(AVUTIL_FOUND AND AVDEVICE_FOUND AND SWSCALE_FOUND AND AVFORMAT_FOUND AND AVCODEC_FOUND)
    add_definitions(-DWITH_FFMPEG -D__STDC_CONSTANT_MACROS)
endif()

if(${ENABLE_FONTCONFIG})
    add_definitions(-DUSE_FC)
    pkg_search_module(FONTCONF REQUIRED fontconfig)
    include_directories(${FONTCONF_INCLUDE_DIRS})
    if (APPLE)
        set(FC_FONTS_DIR "<dir>/System/Library/Fonts</dir><dir>/Library/Fonts</dir><dir>~/Library/Fonts</dir>")
        set(FC_CACHE_DIR "<cachedir>~/.cache/fontconfig</cachedir>")
    elseif(WIN32)
        set(FC_FONTS_DIR "<dir>WINDOWSFONTDIR</dir>")
        set(FC_CACHE_DIR "<cachedir>LOCAL_APPDATA_FONTCONFIG_CACHE</cachedir>")
    else()
        set(FC_FONTS_DIR "<dir>/usr/share/fonts/OTF</dir><dir>/usr/share/fonts/TTF</dir><dir>/usr/share/fonts/Type1</dir><dir>/usr/share/fonts/Speedo</dir><dir>/usr/share/fonts/cyrillic</dir><dir>/usr/share/fonts/misc</dir><dir prefix=\"xdg\">fonts</dir>")
        set(FC_CACHE_DIR "<cachedir prefix=\"xdg\">fontconfig</cachedir><cachedir>~/.fontconfig</cachedir>")
    endif()
    configure_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/share/fonts.conf.in
        ${CMAKE_CURRENT_BINARY_DIR}/etc/fonts/fonts.conf
        @ONLY)
endif()

include_directories(
    app
    app/layers
    app/widgets
    app/dialogs
    lib
    ${MAGICK_INCLUDE_DIRS}
    ${LCMS2_INCLUDE_DIRS}
    #${LUA_INCLUDE_DIRS}
)

add_compile_options(-Wall -Wextra)
if(CMAKE_BUILD_TYPE MATCHES "^(release|Release|RELEASE)$")
    add_definitions(-DQT_NO_DEBUG_OUTPUT)
else()
    add_definitions(-DQT_MESSAGELOGCONTEXT)
endif()
if (${DEBUG_TILES})
    add_definitions(-DSHOW_TILES)
endif()

if(APPLE)
    set(ICON_FILE ${CMAKE_CURRENT_SOURCE_DIR}/share/icons/${PROJECT_NAME}.icns)
    #set(CMAKE_OSX_DEPLOYMENT_TARGET "10.7")
    set(MACOSX_BUNDLE_BUNDLE_NAME ${PROJECT_NAME})
    set(MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION})
    set(MACOSX_BUNDLE_LONG_VERSION_STRING ${PROJECT_VERSION})
    set(MACOSX_BUNDLE_SHORT_VERSION_STRING "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}")
    set(MACOSX_BUNDLE_COPYRIGHT ${COPYRIGHT})
    set(MACOSX_BUNDLE_GUI_IDENTIFIER ${IDENTIFIER})
    set(MACOSX_BUNDLE_ICON_FILE ${PROJECT_NAME})
    set_source_files_properties(${ICON_FILE} PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")
    set(CMAKE_INSTALL_RPATH "@executable_path/../Frameworks")
endif()

if(WIN32)
    set(ICON_FILE ${CMAKE_CURRENT_SOURCE_DIR}/share/icons/${PROJECT_NAME}.ico)
    set(GUI_TYPE WIN32)
    set(PRODUCT_COMMENTS "${PROJECT_NAME} Image Editor")
    set(PRODUCT_COMPANY_NAME ${COMPANY})
    set(PRODUCT_FILE_DESCRIPTION "${PROJECT_NAME} Image Editor")
    set(PRODUCT_VERSION "${PROJECT_VERSION}")
    set(PRODUCT_INTERNAL_NAME ${PROJECT_NAME})
    set(PRODUCT_COMPANY_COPYRIGHT ${COPYRIGHT})
    set(PRODUCT_ORIGINAL_FILENAME "${PROJECT_NAME}.exe")
    set(PRODUCT_BUNDLE ${PROJECT_NAME})
    set(WINDOWS_RESOURCE_FILE ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.rc)
    configure_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/share/${PROJECT_NAME}.rc.in
        ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.rc
        @ONLY)
    configure_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/share/${PROJECT_NAME}.iss.in
        ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.iss
        @ONLY)
endif()



#add_library(
#    ${PROJECT_NAME}LuaMagick
#    SHARED
#    ${CMAKE_CURRENT_SOURCE_DIR}/bindings/lua/src/imagick.c)
#target_compile_options(${PROJECT_NAME}LuaMagick PRIVATE ${MAGICK_STATIC_CFLAGS})
#target_link_libraries(
#    ${PROJECT_NAME}LuaMagick
#    ${MAGICK_STATIC_LIBRARIES}
#    ${MAGICK_STATIC_LDFLAGS}
#    ${LUA_LIBRARIES})

if(BUILD_LIBRARY)
    add_library(
        ${PROJECT_NAME}Api
        SHARED
        ${SOURCES_LIB}
        ${HEADERS_LIB}
        ${HEADERS_API})
    set_target_properties(
        ${PROJECT_NAME}Api
        PROPERTIES
        VERSION
        ${PROJECT_VERSION})
    set_target_properties(
        ${PROJECT_NAME}Api
        PROPERTIES
        SOVERSION
        ${PROJECT_VERSION_MAJOR})
    target_compile_options(${PROJECT_NAME}Api PRIVATE ${MAGICK_STATIC_CFLAGS})
    target_link_libraries(
        ${PROJECT_NAME}Api
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets
        Qt5::Concurrent
        ${MAGICK_STATIC_LIBRARIES}
        ${LCMS2_LIBRARIES}
        ${MAGICK_STATIC_LDFLAGS}
        ${LCMS2_LDFLAGS})
    if(AVUTIL_FOUND AND AVDEVICE_FOUND AND SWSCALE_FOUND AND AVFORMAT_FOUND  AND AVCODEC_FOUND)
        target_link_libraries(
            ${PROJECT_NAME}Api
            ${AVUTIL_LIBRARIES}
            ${AVDEVICE_LIBRARIES}
            ${SWSCALE_LIBRARIES}
            ${AVFORMAT_LIBRARIES}
            ${AVCODEC_LIBRARIES})
    endif()
    #if(APPLE)
    #    target_link_libraries(${PROJECT_NAME}Api ${OpenMP_CXX_LIBRARIES})
    #endif()
else()
    set(SOURCES_APP ${SOURCES_APP} ${SOURCES_LIB})
    set(HEADERS_APP ${HEADERS_APP} ${HEADERS_LIB})
endif()

if(APPLE)
    add_executable(${PROJECT_NAME} MACOSX_BUNDLE ${ICON_FILE} ${SOURCES_APP} ${HEADERS_APP} ${HEADERS_API} ${RESOURCES})
elseif(WIN32)
    add_executable(${PROJECT_NAME} WIN32 ${ICON_FILE} ${SOURCES_APP} ${HEADERS_APP} ${HEADERS_API} ${RESOURCES})
else()
    add_executable(${PROJECT_NAME} ${SOURCES_APP} ${HEADERS_APP} ${HEADERS_API} ${RESOURCES})
endif()

target_compile_options(${PROJECT_NAME} PRIVATE ${MAGICK_STATIC_CFLAGS})
target_link_libraries(
    ${PROJECT_NAME}
    Qt5::Core
    Qt5::Gui
    Qt5::Widgets
    Qt5::Concurrent
    ${MAGICK_STATIC_LIBRARIES}
    ${LCMS2_LIBRARIES}
    ${MAGICK_STATIC_LDFLAGS}
    ${LCMS2_LDFLAGS})
if(AVUTIL_FOUND AND AVDEVICE_FOUND AND SWSCALE_FOUND AND AVFORMAT_FOUND AND AVCODEC_FOUND)
    target_link_libraries(
        ${PROJECT_NAME}
        ${AVUTIL_LIBRARIES}
        ${AVDEVICE_LIBRARIES}
        ${SWSCALE_LIBRARIES}
        ${AVFORMAT_LIBRARIES}
        ${AVCODEC_LIBRARIES})
endif()
if(BUILD_LIBRARY)
    target_link_libraries(${PROJECT_NAME} ${PROJECT_NAME}Api)
endif()
if(${ENABLE_FONTCONFIG})
    target_link_libraries(${PROJECT_NAME} ${FONTCONF_LDFLAGS})
endif()
#if(APPLE)
#    target_link_libraries(${PROJECT_NAME} ${OpenMP_CXX_LIBRARIES})
#endif()
if(${LINUX_DEPLOY})
    target_link_libraries(${PROJECT_NAME} ${CMAKE_THREAD_LIBS_INIT})
endif()

if(MINGW)
    set(MINGW_ROOT "/opt/Cyan-mxe/usr/x86_64-w64-mingw32.shared" CACHE STRING "MINGW/MXE root dir")
    set(QT_ROOT "${MINGW_ROOT}/qt5" CACHE STRING "MINGW/MXE Qt root dir")
    set(
        DLLS
        ${MINGW_ROOT}/bin/libbz2.dll
        ${MINGW_ROOT}/bin/libjpeg-9.dll
        ${MINGW_ROOT}/bin/libpangowin32-1.0-0.dll
        ${MINGW_ROOT}/bin/libwinpthread-1.dll
        ${MINGW_ROOT}/bin/libcairo-2.dll
        ${MINGW_ROOT}/bin/libharfbuzz-0.dll
        ${MINGW_ROOT}/bin/liblcms2-2.dll
        ${MINGW_ROOT}/bin/libpcre16-0.dll
        ${MINGW_ROOT}/bin/libexpat-1.dll
        ${MINGW_ROOT}/bin/libiconv-2.dll
        ${MINGW_ROOT}/bin/liblzma-5.dll
        ${MINGW_ROOT}/bin/libpcre-1.dll
        ${MINGW_ROOT}/bin/libffi-6.dll
        ${MINGW_ROOT}/bin/libMagick++-7.Q16HDRI-Cyan-4.dll
        ${MINGW_ROOT}/bin/libpcre2-16-0.dll
        ${MINGW_ROOT}/bin/libfontconfig-1.dll
        ${MINGW_ROOT}/bin/libMagickCore-7.Q16HDRI-Cyan-6.dll
        ${MINGW_ROOT}/bin/libpixman-1-0.dll
        ${MINGW_ROOT}/bin/libfreetype-6.dll
        ${MINGW_ROOT}/bin/libMagickWand-7.Q16HDRI-Cyan-6.dll
        ${MINGW_ROOT}/bin/libpng16-16.dll
        ${MINGW_ROOT}/bin/zlib1.dll
        ${MINGW_ROOT}/bin/libopenjp2.dll
        ${MINGW_ROOT}/bin/libstdc++-6.dll
        ${MINGW_ROOT}/bin/libglib-2.0-0.dll
        ${MINGW_ROOT}/bin/libpango-1.0-0.dll
        ${MINGW_ROOT}/bin/libtiff-5.dll
        ${MINGW_ROOT}/bin/libgobject-2.0-0.dll
        ${MINGW_ROOT}/bin/libpangocairo-1.0-0.dll
        ${MINGW_ROOT}/bin/libwebp-7.dll
        ${MINGW_ROOT}/bin/libgomp-1.dll
        ${MINGW_ROOT}/bin/libintl-8.dll
        ${MINGW_ROOT}/bin/libpangoft2-1.0-0.dll
        ${MINGW_ROOT}/bin/libwebpmux-3.dll
        ${MINGW_ROOT}/bin/avcodec-58.dll
        ${MINGW_ROOT}/bin/avdevice-58.dll
        ${MINGW_ROOT}/bin/avformat-58.dll
        ${MINGW_ROOT}/bin/avutil-56.dll
        ${MINGW_ROOT}/bin/avresample-4.dll
        ${MINGW_ROOT}/bin/swscale-5.dll
        ${MINGW_ROOT}/bin/swresample-3.dll)
    if(${MINGW32})
        list(APPEND DLLS ${MINGW_ROOT}/bin/libgcc_s_sjlj-1.dll)
    else()
        list(APPEND DLLS ${MINGW_ROOT}/bin/libgcc_s_seh-1.dll)
    endif()
    set(
        QT_DLLS
        ${QT_ROOT}/bin/Qt5Concurrent.dll
        ${QT_ROOT}/bin/Qt5Core.dll
        ${QT_ROOT}/bin/Qt5Gui.dll
        ${QT_ROOT}/bin/Qt5Widgets.dll)
    add_custom_command(
        TARGET
        ${PROJECT_NAME}
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        ${DLLS}
        ${CMAKE_CURRENT_BINARY_DIR}/
        COMMAND ${CMAKE_COMMAND} -E copy
        ${QT_DLLS}
        ${CMAKE_CURRENT_BINARY_DIR}/)
    configure_file(${QT_ROOT}/plugins/platforms/qwindows.dll ${CMAKE_CURRENT_BINARY_DIR}/platforms/qwindows.dll COPYONLY)
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/COPYING ${CMAKE_CURRENT_BINARY_DIR}/docs/COPYING COPYONLY)
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/share/icons/${PROJECT_NAME}.ico ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.ico COPYONLY)
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/README.md ${CMAKE_CURRENT_BINARY_DIR}/docs/README.md COPYONLY)
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/docs/ChangeLog.md ${CMAKE_CURRENT_BINARY_DIR}/docs/ChangeLog.md COPYONLY)
    #configure_file(${CMAKE_CURRENT_SOURCE_DIR}/docs/FatCow.txt ${CMAKE_CURRENT_BINARY_DIR}/docs/FatCow.txt COPYONLY)
    #configure_file(${CMAKE_CURRENT_SOURCE_DIR}/docs/basICColor.txt ${CMAKE_CURRENT_BINARY_DIR}/docs/basICColor.txt COPYONLY)
    #configure_file(${CMAKE_CURRENT_SOURCE_DIR}/docs/Nokia.txt ${CMAKE_CURRENT_BINARY_DIR}/docs/Nokia.txt COPYONLY)
    configure_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/share/icc/ISOcoated_v2_300_bas.icc
        ${CMAKE_CURRENT_BINARY_DIR}/profiles/ISOcoated_v2_300_bas.icc COPYONLY)
    configure_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/share/icc/ISOcoated_v2_grey1c_bas.icc
        ${CMAKE_CURRENT_BINARY_DIR}/profiles/ISOcoated_v2_grey1c_bas.icc COPYONLY)
    configure_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/share/icc/SWOP2006_Coated5v2.icc
        ${CMAKE_CURRENT_BINARY_DIR}/profiles/SWOP2006_Coated5v2.icc COPYONLY)
    configure_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/share/icc/sRGB2014.icc
        ${CMAKE_CURRENT_BINARY_DIR}/profiles/sRGB2014.icc COPYONLY)
    configure_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/share/icc/sRGB_IEC61966-2-1_no_black_scaling.icc
        ${CMAKE_CURRENT_BINARY_DIR}/profiles/sRGB_IEC61966-2-1_no_black_scaling.icc COPYONLY)
    configure_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/share/icc/sRGB_v4_ICC_preference.icc
        ${CMAKE_CURRENT_BINARY_DIR}/profiles/sRGB_v4_ICC_preference.icc COPYONLY)
endif()


if(UNIX AND NOT APPLE)
    include(GNUInstallDirs)
    if(BUILD_LIBRARY)
        if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
            set(LIB_INSTALL_DIR ${CMAKE_INSTALL_LIBDIR})
        else()
            set(LIB_INSTALL_DIR ${CMAKE_INSTALL_FULL_LIBDIR})
        endif()
        install(TARGETS ${PROJECT_NAME}Api DESTINATION ${LIB_INSTALL_DIR})
    endif()
    install(TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_INSTALL_BINDIR})
    install(
        DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/share/icons/hicolor/128x128
        DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/icons/hicolor)
    install(
        DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/share/icons/hicolor/160x160
        DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/icons/hicolor)
    install(
        FILES ${CMAKE_CURRENT_SOURCE_DIR}/share/icons/hicolor/16x16/apps/${PROJECT_NAME}.png
        DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/icons/hicolor/16x16/apps)
    install(
        DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/share/icons/hicolor/192x192
        DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/icons/hicolor)
    install(
        DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/share/icons/hicolor/20x20
        DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/icons/hicolor)
    install(
        DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/share/icons/hicolor/24x24
        DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/icons/hicolor)
    install(
        DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/share/icons/hicolor/256x256
        DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/icons/hicolor)
    install(
        FILES ${CMAKE_CURRENT_SOURCE_DIR}/share/icons/hicolor/32x32/apps/${PROJECT_NAME}.png
        DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/icons/hicolor/32x32/apps)
    install(
        DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/share/icons/hicolor/36x36
        DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/icons/hicolor)
    install(
        DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/share/icons/hicolor/48x48
        DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/icons/hicolor)
    install(
        DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/share/icons/hicolor/512x512
        DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/icons/hicolor)
    install(
        DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/share/icons/hicolor/64x64
        DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/icons/hicolor)
    install(
        DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/share/icons/hicolor/72x72
        DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/icons/hicolor)
    install(
        DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/share/icons/hicolor/96x96
        DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/icons/hicolor)
    #install(FILES ${HEADERS_API} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME})
    install(
        FILES ${CMAKE_CURRENT_SOURCE_DIR}/share/miff.xml
        DESTINATION
        ${CMAKE_INSTALL_DATAROOTDIR}/mime/packages)
    install(
        FILES
        ${CMAKE_CURRENT_SOURCE_DIR}/share/${PROJECT_NAME}.desktop
        DESTINATION
        ${CMAKE_INSTALL_DATAROOTDIR}/applications)
    install(
        FILES
        ${CMAKE_CURRENT_SOURCE_DIR}/share/icc/ISOcoated_v2_300_bas.icc
        ${CMAKE_CURRENT_SOURCE_DIR}/share/icc/ISOcoated_v2_grey1c_bas.icc
        ${CMAKE_CURRENT_SOURCE_DIR}/share/icc/SWOP2006_Coated5v2.icc
        ${CMAKE_CURRENT_SOURCE_DIR}/share/icc/sRGB2014.icc
        ${CMAKE_CURRENT_SOURCE_DIR}/share/icc/sRGB_IEC61966-2-1_no_black_scaling.icc
        ${CMAKE_CURRENT_SOURCE_DIR}/share/icc/sRGB_v4_ICC_preference.icc
        DESTINATION
        ${CMAKE_INSTALL_DATAROOTDIR}/${PROJECT_NAME}/profiles)
    install(
        FILES
        ${CMAKE_CURRENT_SOURCE_DIR}/COPYING
        ${CMAKE_CURRENT_SOURCE_DIR}/README.md
        ${CMAKE_CURRENT_SOURCE_DIR}/docs/ChangeLog.md
        #${CMAKE_CURRENT_SOURCE_DIR}/docs/Nokia.txt
        #${CMAKE_CURRENT_SOURCE_DIR}/docs/FatCow.txt
        #${CMAKE_CURRENT_SOURCE_DIR}/docs/basICColor.txt
        #${CMAKE_CURRENT_SOURCE_DIR}/docs/Nokia.txt
        DESTINATION
        ${CMAKE_INSTALL_DOCDIR}-${PROJECT_VERSION})
endif()


# Include Cyan Color Triangle Widget
if(BUILD_WIDGET_PLUGIN_COLOR_TRIANGLE)
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/plugins/CyanColorTriangle)
endif()

# Include Cyan Widget Example Widget
if(BUILD_WIDGET_PLUGIN_EXAMPLE)
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/plugins/CyanWidgetExample)
endif()
