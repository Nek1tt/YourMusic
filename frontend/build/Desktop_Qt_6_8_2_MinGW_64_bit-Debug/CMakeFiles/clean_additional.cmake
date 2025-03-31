# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\musicproject_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\musicproject_autogen.dir\\ParseCache.txt"
  "musicproject_autogen"
  )
endif()
