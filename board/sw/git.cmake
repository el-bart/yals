file(WRITE "${CMAKE_BINARY_DIR}/version_proxy.cmake"
"execute_process(COMMAND
  git describe --always --abbrev=10 --dirty
  WORKING_DIRECTORY \"${CMAKE_SOURCE_DIR}\"
  OUTPUT_VARIABLE GIT_VERSION_INFO
  OUTPUT_STRIP_TRAILING_WHITESPACE
)
CONFIGURE_FILE(\"\${SRC}\" \"\${DST}\" @ONLY)
")

include_directories(${CMAKE_BINARY_DIR})

add_custom_target(version_proxy
    "${CMAKE_COMMAND}" -D "SRC=${CMAKE_CURRENT_SOURCE_DIR}/Utils/version.cpp.in"
                       -D "DST=${CMAKE_CURRENT_BINARY_DIR}/Utils/version.cpp"
                       -P "${CMAKE_BINARY_DIR}/version_proxy.cmake"
)

#configure_file("${CMAKE_CURRENT_SOURCE_DIR}/Utils/version.cpp.in"
#               "${CMAKE_CURRENT_BINARY_DIR}/Utils/version.cpp"
#                @ONLY)
