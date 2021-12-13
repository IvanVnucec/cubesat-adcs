cmake_minimum_required(VERSION 3.15.3)

project(cubesat-adcs)

enable_language(C CXX ASM)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(STM32CUBEMX_GENERATED_FILES
    core/Startup/startup_stm32l412kbux.s
    core/Src/main.c
    core/Src/gpio.c
    core/Src/freertos.c
    core/Src/i2c.c
    core/Src/spi.c
    core/Src/tim.c
    core/Src/usart.c
    core/Src/stm32l4xx_it.c
    core/Src/stm32l4xx_hal_msp.c
    core/Src/stm32l4xx_hal_timebase_tim.c
    drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal.c
    drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_i2c.c
    drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_i2c_ex.c
    drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_rcc.c
    drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_rcc_ex.c
    drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_flash.c
    drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_flash_ex.c
    drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_flash_ramfunc.c
    drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_gpio.c
    drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_dma.c
    drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_dma_ex.c
    drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_pwr.c
    drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_pwr_ex.c
    drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_cortex.c
    drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_exti.c
    drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_spi.c
    drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_spi_ex.c
    drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_tim.c
    drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_tim_ex.c
    drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_uart.c
    drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_uart_ex.c
    drivers/mpu9250/mpu9250.cpp
    drivers/i2c_user/i2c_user.cpp
    drivers/uart_user/uart_user.cpp
    drivers/pwm_user/pwm_user.cpp
    core/Src/syscalls.c
    core/Src/sysmem.c
    core/Src/system_stm32l4xx.c
    Middlewares/printf-5.1.0/src/printf.c
    Middlewares/parser/parser.cpp
    Middlewares/parser/callbacks.cpp
    Middlewares/fault_handling/fault_handling.cpp
    Middlewares/adcs/adcs_tasks.cpp
    Middlewares/adcs/inertial_meas_unit.cpp
    Middlewares/adcs/optimal_request_interface.cpp
    Middlewares/Third_Party/FreeRTOS/Source/croutine.c
    Middlewares/Third_Party/FreeRTOS/Source/event_groups.c
    Middlewares/Third_Party/FreeRTOS/Source/list.c
    Middlewares/Third_Party/FreeRTOS/Source/queue.c
    Middlewares/Third_Party/FreeRTOS/Source/stream_buffer.c
    Middlewares/Third_Party/FreeRTOS/Source/tasks.c
    Middlewares/Third_Party/FreeRTOS/Source/timers.c
    Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2/cmsis_os2.c
    Middlewares/Third_Party/FreeRTOS/Source/portable/MemMang/heap_4.c
    Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F/port.c
)

set(EXECUTABLE ${PROJECT_NAME}.elf)

add_executable(${EXECUTABLE} ${STM32CUBEMX_GENERATED_FILES})

include("cmake/STM32_flash.cmake")
include("cmake/STM32_debug_gdb.cmake")
include("cmake/clang_format_run.cmake")

target_compile_definitions(${EXECUTABLE} PRIVATE
    $<$<CONFIG:Debug>:DEBUG>
    USE_HAL_DRIVER
    STM32L412xx
)

target_include_directories(${EXECUTABLE} PRIVATE
    core/Inc
    drivers/STM32L4xx_HAL_Driver/Inc
    drivers/STM32L4xx_HAL_Driver/Inc/Legacy
    Middlewares
    Middlewares/parser
    Middlewares/optimal_request
    Middlewares/fault_handling
    Middlewares/adcs
    Middlewares/Third_Party/FreeRTOS/Source/include
    Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2
    Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F
    drivers/mpu9250
    drivers/i2c_user
    drivers/uart_user
    drivers/pwm_user
    drivers/CMSIS/Device/ST/STM32L4xx/Include
    drivers/CMSIS/Include
)

target_compile_options(${EXECUTABLE} PRIVATE
    # COMMON FLAGS
    -mcpu=cortex-m4
    -g3
    -specs=nano.specs
    -mthumb
    -mfpu=fpv4-sp-d16
    -mfloat-abi=hard

    # ASM FLAGS
    $<$<COMPILE_LANGUAGE:ASM>:-x assembler-with-cpp>

    # C FLAGS
    $<$<COMPILE_LANGUAGE:C>:$<$<CONFIG:Debug>:-O0>>
    $<$<COMPILE_LANGUAGE:C>:-ffunction-sections>
    $<$<COMPILE_LANGUAGE:C>:-fdata-sections>
    $<$<COMPILE_LANGUAGE:C>:-Wall>
    $<$<COMPILE_LANGUAGE:C>:-Wmissing-include-dirs>
    $<$<COMPILE_LANGUAGE:C>:-fstack-usage>

    # CXX FLAGS
    $<$<COMPILE_LANGUAGE:C>:$<$<CONFIG:Debug>:-O0>>
    $<$<COMPILE_LANGUAGE:CXX>:-ffunction-sections>
    $<$<COMPILE_LANGUAGE:CXX>:-fdata-sections>
    $<$<COMPILE_LANGUAGE:CXX>:-fno-exceptions>
    $<$<COMPILE_LANGUAGE:CXX>:-fno-rtti>
    $<$<COMPILE_LANGUAGE:CXX>:-fno-use-cxa-atexit>
    $<$<COMPILE_LANGUAGE:CXX>:-Wall>
    $<$<COMPILE_LANGUAGE:CXX>:-Wmissing-include-dirs>
    $<$<COMPILE_LANGUAGE:CXX>:-fstack-usage>
)

target_link_options(${EXECUTABLE} PRIVATE
    -mcpu=cortex-m4 
    -T${CMAKE_SOURCE_DIR}/Linker/STM32L412KBUX_FLASH.ld
    --specs=nosys.specs 
    --specs=nano.specs 
    -Wl,-Map=${PROJECT_NAME}.map,--cref
    -Wl,--gc-sections 
    -static 
    -mfpu=fpv4-sp-d16 
    -mfloat-abi=hard 
    -mthumb 
    -Wl,--start-group 
    -lc 
    -lm 
    -lstdc++ 
    -lsupc++ 
    -Wl,--end-group
)

# Print executable size
add_custom_command(TARGET ${EXECUTABLE}
    POST_BUILD
    COMMAND arm-none-eabi-size ${EXECUTABLE}
)

# Create hex file
add_custom_command(TARGET ${EXECUTABLE}
    POST_BUILD
    COMMAND arm-none-eabi-objcopy -O ihex ${EXECUTABLE} ${PROJECT_NAME}.hex
    COMMAND arm-none-eabi-objcopy -O binary ${EXECUTABLE} ${PROJECT_NAME}.bin
)

set_target_properties(${EXECUTABLE} PROPERTIES ADDITIONAL_CLEAN_FILES
        "${PROJECT_NAME}.bin;${PROJECT_NAME}.hex;${PROJECT_NAME}.map")

DEBUG_GDB_TARGET("${PROJECT_NAME}.hex")
ADD_FLASH_TARGET("${PROJECT_NAME}.elf")
ADD_CLANG_FORMAT_TARGET()
