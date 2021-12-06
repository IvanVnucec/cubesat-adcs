# README

## Build
1. Install STM32CubeIDE Version: 1.7.0
2. Clone project
3. Import cloned project in IDE
4. Build Debug configuration

## Development
### Code structure
- Middlewares/optimal_request - Optimal request algorithm copied from https://github.com/IvanVnucec/Optimal-REQUEST
- Middlewares/adcs - Files for Attitude determination and control system files
- Drivers/mpu9250 - MPU9250 IMU drivers

### Code formatting
1. Install LLVM package https://prereleases.llvm.org/win-snapshots/LLVM-12.0.0-6923b0a7-win64.exe
2. Install CppStyle 1.5.0.0 on Eclipse Marketplace https://marketplace.eclipse.org/content/cppstyle
3. Go to Windows - Preferences - C/C++ - CppStyle and 
	- add Clang-format path and
	- check Run clang-format on file save

### Problem with the UART Baudrate
STM32 Nucleo board for STM32L412 is configured by default to use internal clock. We had problems with the UART baudrate. When configured to 115200 bits/s we would have about 108000 bits/s. In order to configure Nucleo board to use external crystal (from the STM Link) we have unsoldered SB5 and SB7 solder bridges and soldered SB17 solder bridge. Doing that we have introduced a bug where initial pins PB7 and PB6 for UART did not work, they were in high state when transmitting. To fix the bug we have changed pinout to PA10 and PA9 for the UART1. 
	
## Usage
### Parser
To use parser functionalities:
1. Connect to the bluetooth module on the board with some terminal program. I tend to use the "Serial Bluetooth Terminal 1.36" on my Android phone to send and receive character commands. 
2. Add your callback function definitions and declarations in the `callbacks.hpp` and `callbacks.cpp` files and also add function pointers to the `m_callbacks` variable in `parser.hpp` file.
3. Send commands as `echo(some text)\n`. Be aware that every parser command, in order to be received, must end with the EOL ('\n') character.
4. Observe incomming messages.
