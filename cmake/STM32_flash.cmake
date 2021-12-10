set(STM32prog openocd.exe)

function(ADD_FLASH_TARGET elf)

    add_custom_target(flash
        DEPENDS ${elf}
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        COMMAND ${STM32prog} "-f" "interface/stlink.cfg" "-f" "target/stm32l4x.cfg" "-c" "program ${elf} verify reset exit"

)

endfunction()
