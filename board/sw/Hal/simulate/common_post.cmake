function (add_binary name)
  add_executable("${name}" ${ARGN})
  target_link_libraries("${name}" hal)
endfunction()
