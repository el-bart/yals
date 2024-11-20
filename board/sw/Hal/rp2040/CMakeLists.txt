add_library(hal placeholder.cpp)
target_link_libraries(hal pico_stdlib)
target_link_libraries(hal hardware_adc)
target_link_libraries(hal hardware_pwm)
target_link_libraries(hal hardware_i2c)

function (add_mtest name)
  add_binary("Hal_${name}.mtest" "Hal/${name}.mtest.cpp")
endfunction()

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/")

add_mtest(EEPROM)
add_mtest(Engine)
add_mtest(Engine_current)
add_mtest(Led)
add_mtest(Position_feedback)
add_mtest(Uart)
add_mtest(Vcc_feedback)

add_mtest(I2C_bus_scan) # rework of a 3rd-party code
