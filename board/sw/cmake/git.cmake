# this is a rework of a concept from:
# https://cmake.org/pipermail/cmake/2010-July/038015.html

# defines always-outdated target, that will however only change files when new content is available
add_custom_target(version_proxy
 "${CMAKE_COMMAND}" -D "SRC=${CMAKE_SOURCE_DIR}/Utils/version.cpp.in"
                    -D "DST=${CMAKE_BINARY_DIR}/Utils/version.cpp"
                    -P "${CMAKE_SOURCE_DIR}/cmake/version_proxy.cmake"
  SOURCE     "${CMAKE_SOURCE_DIR}/Utils/version.cpp.in"
  BYPRODUCTS "${CMAKE_BINARY_DIR}/Utils/version.cpp"
)
