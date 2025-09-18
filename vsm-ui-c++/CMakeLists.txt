cmake_minimum_required(VERSION 3.16)

project(vsm VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Detect Windows
if(WIN32)
    set(WIN_BUILD TRUE)
endif()

# Find Qt
find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets Network)

# Add QHotkey
add_subdirectory(external/QHotkey)

# Your sources
set(PROJECT_SOURCES
    main.cpp
    vsm.cpp
    vsm.h
    vsm.ui
    SearchBar.h
    SearchBar.cpp
    ApiManager.h
    ApiManager.cpp
)

# Qt executable
qt_add_executable(vsm
    MANUAL_FINALIZATION
    ${PROJECT_SOURCES}
)

# Link libraries
target_link_libraries(vsm PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::Network
    QHotkey::QHotkey
)

# Windows-specific executable flag
if(WIN32)
    set_target_properties(vsm PROPERTIES
        WIN32_EXECUTABLE TRUE
    )
endif()

qt_finalize_executable(vsm)
