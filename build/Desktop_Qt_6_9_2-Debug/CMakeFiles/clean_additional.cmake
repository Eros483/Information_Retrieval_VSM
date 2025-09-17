# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles/vsm_autogen.dir/AutogenUsed.txt"
  "CMakeFiles/vsm_autogen.dir/ParseCache.txt"
  "external/QHotkey/CMakeFiles/qhotkey_autogen.dir/AutogenUsed.txt"
  "external/QHotkey/CMakeFiles/qhotkey_autogen.dir/ParseCache.txt"
  "external/QHotkey/qhotkey_autogen"
  "vsm_autogen"
  )
endif()
