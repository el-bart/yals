include(Hal/rp2040/clocks.cmake)
pico_sdk_init()

function (add_binary name)
  add_executable("${name}" ${ARGN})
  target_link_libraries("${name}" hal)
  # create map/bin/hex/uf2 file etc.
  pico_add_extra_outputs("${name}")
endfunction()
