cmake_minimum_required(VERSION 3.20)
function (add_mtest name)
  add_binary("Utils_${name}.mtest" "${name}.mtest.cpp")
endfunction()

add_mtest(Engine_controller)
add_mtest(Engine_controller_auto_jog)
add_mtest(Engine_controller_rapid_change)
