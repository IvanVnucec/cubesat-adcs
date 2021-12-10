set(STM32prog openocd.exe)

function(DEBUG_GDB_TARGET hex)

    add_custom_target(debug_gdb
        DEPENDS ${hex}
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        COMMENT "Flashing to STM32"
        COMMAND ${STM32prog} "-c" "gdb_port 50000" "-c" "tcl_port 50001" "-c" "telnet_port 50002" "-s" "${hex}" "-f" "interface/stlink.cfg" "-f" "target/stm32l4x.cfg"
)

endfunction()
