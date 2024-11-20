include_directories(${CMAKE_BINARY_DIR})

add_custom_target(version_proxy
 "${CMAKE_COMMAND}" -D "SRC=${CMAKE_SOURCE_DIR}/Utils/version.cpp.in"
                    -D "DST=${CMAKE_BINARY_DIR}/Utils/version.cpp"
                    -P "${CMAKE_SOURCE_DIR}/cmake/version_proxy.cmake"
  SOURCE     "${CMAKE_SOURCE_DIR}/Utils/version.cpp.in"
  BYPRODUCTS "${CMAKE_BINARY_DIR}/Utils/version.cpp"
)
