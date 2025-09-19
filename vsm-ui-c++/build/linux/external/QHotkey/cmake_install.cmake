# Install script for directory: /home/s-uryansh/Desktop/College/CSD358_IR/Assignment1/Information_Retrieval_VSM/vsm-ui-c++/external/QHotkey

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "1")
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

# Set path to fallback-tool for dependency-resolution.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "/usr/bin/objdump")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "/home/s-uryansh/Desktop/College/CSD358_IR/Assignment1/Information_Retrieval_VSM/vsm-ui-c++/build/linux/external/QHotkey/libqhotkey.a")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE FILE FILES
    "/home/s-uryansh/Desktop/College/CSD358_IR/Assignment1/Information_Retrieval_VSM/vsm-ui-c++/external/QHotkey/QHotkey/qhotkey.h"
    "/home/s-uryansh/Desktop/College/CSD358_IR/Assignment1/Information_Retrieval_VSM/vsm-ui-c++/external/QHotkey/QHotkey/QHotkey"
    )
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/QHotkey" TYPE FILE FILES "/home/s-uryansh/Desktop/College/CSD358_IR/Assignment1/Information_Retrieval_VSM/vsm-ui-c++/build/linux/external/QHotkey/QHotkeyConfigVersion.cmake")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/QHotkey/QHotkeyConfig.cmake")
    file(DIFFERENT _cmake_export_file_changed FILES
         "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/QHotkey/QHotkeyConfig.cmake"
         "/home/s-uryansh/Desktop/College/CSD358_IR/Assignment1/Information_Retrieval_VSM/vsm-ui-c++/build/linux/external/QHotkey/CMakeFiles/Export/780296f38fabd842a663f9c594181ed7/QHotkeyConfig.cmake")
    if(_cmake_export_file_changed)
      file(GLOB _cmake_old_config_files "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/QHotkey/QHotkeyConfig-*.cmake")
      if(_cmake_old_config_files)
        string(REPLACE ";" ", " _cmake_old_config_files_text "${_cmake_old_config_files}")
        message(STATUS "Old export file \"$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/QHotkey/QHotkeyConfig.cmake\" will be replaced.  Removing files [${_cmake_old_config_files_text}].")
        unset(_cmake_old_config_files_text)
        file(REMOVE ${_cmake_old_config_files})
      endif()
      unset(_cmake_old_config_files)
    endif()
    unset(_cmake_export_file_changed)
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/QHotkey" TYPE FILE FILES "/home/s-uryansh/Desktop/College/CSD358_IR/Assignment1/Information_Retrieval_VSM/vsm-ui-c++/build/linux/external/QHotkey/CMakeFiles/Export/780296f38fabd842a663f9c594181ed7/QHotkeyConfig.cmake")
  if(CMAKE_INSTALL_CONFIG_NAME MATCHES "^()$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/QHotkey" TYPE FILE FILES "/home/s-uryansh/Desktop/College/CSD358_IR/Assignment1/Information_Retrieval_VSM/vsm-ui-c++/build/linux/external/QHotkey/CMakeFiles/Export/780296f38fabd842a663f9c594181ed7/QHotkeyConfig-noconfig.cmake")
  endif()
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
if(CMAKE_INSTALL_LOCAL_ONLY)
  file(WRITE "/home/s-uryansh/Desktop/College/CSD358_IR/Assignment1/Information_Retrieval_VSM/vsm-ui-c++/build/linux/external/QHotkey/install_local_manifest.txt"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
endif()
