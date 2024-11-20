# regular generator. to be called from git.cmake

execute_process(COMMAND git describe --always --abbrev=10 --dirty
  WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
  OUTPUT_VARIABLE GIT_VERSION_INFO
  OUTPUT_STRIP_TRAILING_WHITESPACE
)

configure_file("${SRC}" "${DST}" @ONLY)
