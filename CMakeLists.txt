cmake_minimum_required(VERSION 3.16)

project(ChessQt VERSION 0.1 LANGUAGES CXX)

# Enable automatic Qt processing
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include source directory
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

# Find required Qt packages
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Network)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Network)

set(UI_FILES
    game_client/chess_ui/mainwindow.ui
)

# Source files
set(PROJECT_SOURCES
    main.cpp

    game_client/chess_ui/mainwindow.cpp
    game_client/chess_ui/mainwindow.h

    game_client/chess_ui/piece_label.cpp
    game_client/chess_ui/piece_label.h
    game_client/chess_ui/clickable_tile_label.cpp
    game_client/chess_ui/clickable_tile_label.h

    game_client/chess_logic/chess_pieces.cpp
    game_client/chess_logic/chess_pieces.h
    game_client/chess_logic/chess_game.cpp
    game_client/chess_logic/chess_game.h

    game_client/client.cpp
    game_client/client.h

    game_server/server.cpp
    game_server/server.h

    resources.qrc
)

# Add the executable
if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(ChessQt
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
else()
    add_executable(ChessQt
        ${PROJECT_SOURCES}
    )
endif()

# Link required Qt libraries
target_link_libraries(ChessQt
    PRIVATE
        Qt${QT_VERSION_MAJOR}::Widgets
        Qt${QT_VERSION_MAJOR}::Core
        Qt${QT_VERSION_MAJOR}::Network
)

# 🟡 Add this to include auto-generated files like ui_mainwindow.h
target_include_directories(ChessQt
    PRIVATE
        ${CMAKE_CURRENT_BINARY_DIR}
)

# macOS/iOS bundle metadata (optional for Windows)
if(${QT_VERSION} VERSION_LESS 6.1.0)
    set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.ChessQt)
endif()

set_target_properties(ChessQt PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

# Install targets
include(GNUInstallDirs)
install(TARGETS ChessQt
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# Finalize executable if using Qt 6
if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(ChessQt)
endif()
