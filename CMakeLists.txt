cmake_minimum_required(VERSION 3.5)

project(TDEditor VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# QtCreator supports the following variables for Android, which are identical to qmake Android variables.
# Check https://doc.qt.io/qt/deployment-android.html for more information.
# They need to be set before the find_package( ...) calls below.

#if(ANDROID)
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
#    if (ANDROID_ABI STREQUAL "armeabi-v7a")
#        set(ANDROID_EXTRA_LIBS
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libcrypto.so
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libssl.so)
#    endif()
#endif()

set(CMAKE_PREFIX_PATH "C:/Qt/Qt6.1.2/6.1.2/mingw81_64/lib/cmake")

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets REQUIRED)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Interface)

# Set to search in the resources folder for ui files
set(CMAKE_AUTOUIC_SEARCH_PATHS ${PROJECT_SOURCE_DIR}/Resources)

set(PROJECT_SOURCES
   Source/main.cpp
   Source/mainwindow.cpp
   Source/tile.cpp
   Source/tilestore.cpp
   Source/tilesview.cpp
   Source/selectedview.cpp
   Source/mapview.cpp
   Source/newdialog.cpp
   Source/eventseditor.cpp
   Source/spawndelegate.cpp
   Source/percentdelegate.cpp
   Source/settings.cpp
   Source/pixmapstore.cpp
   Source/spawnmgr.cpp
   Source/sprite.cpp
   Source/spritemgr.cpp
   Source/game.cpp
   Source/Enemies/Zombie.cpp
    
   Interface/mainwindow.h
   Interface/tile.h
   Interface/tilestore.h
   Interface/tilesview.h
   Interface/selectedview.h
   Interface/mapview.h
   Interface/newdialog.h
   Interface/eventseditor.h
   Interface/spawndelegate.h
   Interface/percentdelegate.h
   Interface/settings.h
   Interface/pixmapstore.h
   Interface/spawnmgr.h
   Interface/sprite.h
   Interface/spritemgr.h
   Interface/game.h
   Interface/Enemies/Zombie.h

   Resources/mainwindow.ui
   Resources/newdialog.ui
)

qt6_add_resources(Resources/resource.qrc)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(TDEditor
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
else()
    if(ANDROID)
        add_library(TDEditor SHARED
            ${PROJECT_SOURCES}
        )
    else()
        add_executable(TDEditor
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(TDEditor PRIVATE
   Qt${QT_VERSION_MAJOR}::Widgets
)

set_target_properties(TDEditor PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(TDEditor)
endif()

if (NOT DEFINED ENV{ADT_BUILD_SERVER})
   add_custom_command(TARGET TDEditor POST_BUILD
      COMMAND "C:/AdtDevEnv/Core/signtool/signtool.exe" sign /fd SHA256 /f "Code Signing.cer" TDEditor.exe
   )
endif()

