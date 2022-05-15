cmake_minimum_required(VERSION 3.18.4)

if(NOT DEFINED CMAKE_EXPORT_COMPILE_COMMANDS)
	set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)
endif()

if (NOT CMAKE_BUILD_TYPE OR CMAKE_BUILD_TYPE STREQUAL "None")
    set (CMAKE_BUILD_TYPE "Debug")
    message (STATUS "CMAKE_BUILD_TYPE is not set, set to default = ${CMAKE_BUILD_TYPE}")
endif ()

project(cubesat-adcs LANGUAGES C)

enable_language(C ASM)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

set(STM32CUBEMX_GENERATED_FILES
    src/main.c

    src/core/startup_stm32l412kbux.s
    src/core/stm32l4xx_it.c
    src/core/stm32l4xx_hal_msp.c
    src/core/stm32l4xx_hal_timebase_tim.c
    src/core/syscalls.c
    src/core/sysmem.c
    src/core/system_stm32l4xx.c

    src/mcu/mcu.c
    src/mcu/gpio/mcu_gpio.c
    src/mcu/uart/mcu_uart.c
    src/mcu/i2c/mcu_i2c.c
    src/mcu/clock/mcu_clock.c

    src/drivers/drivers.c
    src/drivers/tim/tim.c

    src/drivers/hal/Src/stm32l4xx_hal.c
    src/drivers/hal/Src/stm32l4xx_hal_i2c.c
    src/drivers/hal/Src/stm32l4xx_hal_i2c_ex.c
    src/drivers/hal/Src/stm32l4xx_hal_rcc.c
    src/drivers/hal/Src/stm32l4xx_hal_rcc_ex.c
    src/drivers/hal/Src/stm32l4xx_hal_flash.c
    src/drivers/hal/Src/stm32l4xx_hal_flash_ex.c
    src/drivers/hal/Src/stm32l4xx_hal_flash_ramfunc.c
    src/drivers/hal/Src/stm32l4xx_hal_gpio.c
    src/drivers/hal/Src/stm32l4xx_hal_dma.c
    src/drivers/hal/Src/stm32l4xx_hal_dma_ex.c
    src/drivers/hal/Src/stm32l4xx_hal_pwr.c
    src/drivers/hal/Src/stm32l4xx_hal_pwr_ex.c
    src/drivers/hal/Src/stm32l4xx_hal_cortex.c
    src/drivers/hal/Src/stm32l4xx_hal_exti.c
    src/drivers/hal/Src/stm32l4xx_hal_spi.c
    src/drivers/hal/Src/stm32l4xx_hal_spi_ex.c
    src/drivers/hal/Src/stm32l4xx_hal_tim.c
    src/drivers/hal/Src/stm32l4xx_hal_tim_ex.c
    src/drivers/hal/Src/stm32l4xx_hal_uart.c
    src/drivers/hal/Src/stm32l4xx_hal_uart_ex.c

    src/bsp/bsp.c
    src/bsp/led/bsp_led.c
    
    src/rtos/FreeRTOS/source/croutine.c
    src/rtos/FreeRTOS/source/event_groups.c
    src/rtos/FreeRTOS/source/list.c
    src/rtos/FreeRTOS/source/queue.c
    src/rtos/FreeRTOS/source/stream_buffer.c
    src/rtos/FreeRTOS/source/tasks.c
    src/rtos/FreeRTOS/source/timers.c
    src/rtos/FreeRTOS/source/CMSIS_RTOS_V2/cmsis_os2.c
    src/rtos/FreeRTOS/source/portable/MemMang/heap_4.c
    src/rtos/FreeRTOS/source/portable/GCC/ARM_CM4F/port.c

    src/middlewares/middlewares.c
    src/middlewares/alive/alive.c
    src/middlewares/adcs/adcs.c
    src/middlewares/adcs/adcs_imu.c
    src/middlewares/adcs/adcs_or.c
    src/middlewares/adcs/adcs_rw.c
    src/middlewares/adcs/adcs_pid.c
    src/middlewares/adcs/adcs_reg.c
    src/middlewares/communication/comm.c

    src/libs/printf/src/printf/printf.c
    src/libs/optimal_request/src/opt_req/get_quat_from_K_data.c
    src/libs/optimal_request/src/opt_req/get_quat_from_K_initialize.c
    src/libs/optimal_request/src/opt_req/get_quat_from_K_terminate.c
    src/libs/optimal_request/src/opt_req/get_quat_from_K.c
    src/libs/optimal_request/src/opt_req/optimal_request_init.c
    src/libs/optimal_request/src/opt_req/optimal_request.c
    src/libs/optimal_request/src/opt_req/vecnorm.c
    src/libs/mpu9250/src/mpu9250/mpu9250.c
    src/libs/zs040/src/zs040.c

    src/utils/error/error.c
)

set(EXECUTABLE ${PROJECT_NAME}.elf)

add_executable(${EXECUTABLE} ${STM32CUBEMX_GENERATED_FILES})

target_compile_definitions(${EXECUTABLE} PRIVATE
    $<$<CONFIG:Debug>:DEBUG>
    $<$<CONFIG:Release>:RELEASE>
    USE_HAL_DRIVER
    STM32L412xx
)

target_include_directories(${EXECUTABLE} PRIVATE
    src
    
    src/drivers/hal/Inc
    src/drivers/hal/Inc/Legacy
    src/drivers/cmsis/Device/ST/STM32L4xx/Include
    src/drivers/cmsis/Include

    src/rtos/config
    src/rtos/FreeRTOS/source/include
    src/rtos/FreeRTOS/source/CMSIS_RTOS_V2
    src/rtos/FreeRTOS/source/portable/GCC/ARM_CM4F

    src/libs/zs040/include
    src/libs/printf/src
    src/libs/optimal_request/src
    src/libs/mpu9250/include
)

target_compile_options(${EXECUTABLE} PRIVATE
    # COMMON FLAGS
    -march=armv7e-m
    -g3
    -specs=nano.specs
    -mthumb
    -mfpu=fpv4-sp-d16
    -mfloat-abi=hard
    -Wall
    -Werror
    -ffunction-sections
    -fdata-sections
    -fno-exceptions
    -Wmissing-include-dirs
    -fstack-usage
    $<$<CONFIG:Debug>:-O0>
    $<$<CONFIG:Release>:-Os>

    # C FLAGS
    $<$<COMPILE_LANGUAGE:CXX>:>
)

set(LINKER_SCRIPT "${CMAKE_SOURCE_DIR}/src/linker/STM32L412KBUX_FLASH.ld")

target_link_options(${EXECUTABLE} PRIVATE
    -march=armv7e-m 
    -T${LINKER_SCRIPT}
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
    -Wl,--print-memory-usage
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

set_target_properties(${EXECUTABLE} PROPERTIES
    LINK_DEPENDS ${LINKER_SCRIPT} 
    ADDITIONAL_CLEAN_FILES "${PROJECT_NAME}.bin;${PROJECT_NAME}.hex;${PROJECT_NAME}.map"
)

include("cmake/STM32_flash.cmake")
include("cmake/STM32_debug_gdb.cmake")
include("cmake/clang_format_run.cmake")
include("cmake/check_clang_format.cmake")

DEBUG_GDB_TARGET("${PROJECT_NAME}.hex")
ADD_FLASH_TARGET("${PROJECT_NAME}.elf")
ADD_CLANG_FORMAT_TARGET()
ADD_CHECK_CLANG_FORMAT_TARGET()
