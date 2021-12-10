# README

## Development
Make sure you install all required toolchains listed below. After you have all requirements then you can go to the building stage and finally to the debugging stage.

### Toolchain
- ninja build system (latest version is ok)
- vscode
- mingw (if on windows)
- cmake (minimal version 3.22.0)
- make
- GNU Arm Embedded Toolchain (minimal version 10.3-2021.10)
- clang-format version 12.0.0
- OpenOCD (minimal version 0.11.0)

### Building
1. (if on windows) open mingw console
2. position yourself in the project root directory.  
3. execute the commands below to compile the project.
```console
mkdir build
cd build
cmake -GNinja -DCMAKE_TOOLCHAIN_FILE=../cmake/CMakeSetCompiler.cmake -DCMAKE_BUILD_TYPE=Debug ..
ninja
```

### Debugging
#### With vscode
1. open vscode in project root folder
2. download "Cortex-Debug" extension
3. go to "Run and Debug" and run "Cortex Debug" debug configuration
#### With GDB
1. run `ninja debug_gdb`
2. open gdb in another terminal
3. connect to the running openocd target with `target remote localhost:5000`

### Flashing
1. run `ninja flash` and then 
2. wait for completion

### Code formatting
1. run `ninja clang_format`

### Code structure
- Middlewares/optimal_request - Optimal request algorithm copied from https://github.com/IvanVnucec/Optimal-REQUEST
- Middlewares/adcs - Files for Attitude determination and control system files
- Drivers/mpu9250 - MPU9250 IMU drivers

### Problem with the UART Baudrate
STM32 Nucleo board for STM32L412 is configured by default to use internal clock. We had problems with the UART baudrate. When configured to 115200 bits/s we would have about 108000 bits/s. In order to configure Nucleo board to use external crystal (from the STM Link) we have unsoldered SB5 and SB7 solder bridges and soldered SB17 solder bridge. Doing that we have introduced a bug where initial pins PB7 and PB6 for UART did not work, they were in high state when transmitting. To fix the bug we have changed pinout to PA10 and PA9 for the UART1. 
	
## Usage
### Parser
To use parser functionalities:
1. Connect to the bluetooth module on the board with some terminal program. I tend to use the "Serial Bluetooth Terminal 1.36" on my Android phone to send and receive character commands. 
2. Add your callback function definitions and declarations in the `callbacks.hpp` and `callbacks.cpp` files and also add function pointers to the `m_callbacks` variable in `parser.hpp` file.
3. Send commands as `echo(some text)\n`. Be aware that every parser command, in order to be received, must end with the EOL ('\n') character.
4. Observe incomming messages.
