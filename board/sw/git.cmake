execute_process(COMMAND
  git describe --always --abbrev=10 --dirty
  WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
  OUTPUT_VARIABLE GIT_VERSION_INFO
  OUTPUT_STRIP_TRAILING_WHITESPACE
)

configure_file("${CMAKE_CURRENT_SOURCE_DIR}/Utils/version.cpp.in"
               "${CMAKE_CURRENT_BINARY_DIR}/Utils/version.cpp"
                @ONLY)
