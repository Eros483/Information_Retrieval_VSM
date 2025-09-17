cmake_minimum_required(VERSION 3.16)

project(vsm VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find Qt
find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets Network)

# Add QHotkey
# find_package(QHotkey REQUIRED)
add_subdirectory(external/QHotkey)


# Your sources
set(PROJECT_SOURCES
    main.cpp
    vsm.cpp
    vsm.h
    vsm.ui
)
# add_subdirectory(external/QHotkey)

# Add your executable target
qt_add_executable(vsm
    MANUAL_FINALIZATION
    ${PROJECT_SOURCES}
    SearchBar.h
    SearchBar.cpp
    ApiManager.h
    ApiManager.cpp
)

# Link all needed Qt modules
target_link_libraries(vsm PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::Network
    QHotkey::QHotkey
)



# Bundle properties (optional for macOS/iOS)
set_target_properties(vsm PROPERTIES
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

# Finalize Qt executable3
qt_finalize_executable(vsm)
