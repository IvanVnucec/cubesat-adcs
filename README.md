[![build-and-test](https://github.com/IvanVnucec/cubesat-adcs/actions/workflows/build_and_test.yml/badge.svg)](https://github.com/IvanVnucec/cubesat-adcs/actions/workflows/build_and_test.yml)

# README

## Development
 <b>IMPORTANT:</b> Recommended OS for the development is the [Ubuntu 21.10](https://ubuntu.com/download/desktop). If you don't have a machine with the Ubuntu 21.10, the easiest workaround is to install Ubuntu on a Virtual Machine ([Link for a tutorial](https://www.youtube.com/watch?v=0CgNfilnJPY)).
<p>Make sure you install all required dependencies listed below with written versions (or higher). After you have all the requirements installed, then you can go to the building stage, and finally to the debugging stage.</p>

### Dependencies
- GNU Make (v4.3)
- git (v2.32.0)
- Ninja build system ([v1.10.1](https://github.com/ninja-build/ninja/releases/tag/v1.10.1))
- CMake ([v3.18.4](https://github.com/Kitware/CMake/releases/tag/v3.18.4))
- GNU Arm Embedded Toolchain ([v10.3-2021.10](https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/gnu-rm/downloads))
- ClangFormat ([v13.0.0-2](https://github.com/llvm/llvm-project/releases/tag/llvmorg-12.0.0))
- OpenOCD ([v0.11.0-rc2](https://github.com/xpack-dev-tools/openocd-xpack/releases))
- Cppcheck ([v2.3](https://cppcheck.sourceforge.io/#download))
- Visual Studio Code ([Link](https://code.visualstudio.com/download))
    - Cortex-Debug ([Link](https://marketplace.visualstudio.com/items?itemName=marus25.cortex-debug))
    - C/C++ ([LINK](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cpptools))

### Install dependencies
To install all the dependencies on the Ubuntu 21.10, user can just follow the steps below. Steps below will install all the required tools and VSCode code editor with all the required extensions.
1. Clone project 
2. Position yourself in the project root directory with `cd cubesat-adcs`
3. Install all dependencies by running `make install_deps` (requires sudo privileges)

### Building
1. Clone git submodules with `git submodule update --init --recursive`
2. Setup CMake with `make setup_cmake` (needed only once)
3. Build with `make build` or simply `make`

### Debugging
#### With vscode
1. Open vscode in project root folder with `code .`
2. Go to `Run and Debug` and run `Cortex Debug` debug configuration
#### With GDB
1. run `make debug_gdb`
2. open gdb in another terminal
3. connect to the running openocd target with `target remote localhost:5000`

### Flashing
1. run `make flash` and wait for completion

### Code formatting
1. run `make clang_format`

### Code analysis
1. run `make cppcheck`

### Code structure
- src/middlewares/optimal_request - Optimal request algorithm copied from https://github.com/IvanVnucec/Optimal-REQUEST
- src/middlewares/adcs - Files for Attitude determination and control system files
- src/drivers/user/mpu9250 - MPU9250 IMU drivers

### Problem with the UART Baudrate
STM32 Nucleo board for STM32L412 is configured by default to use internal clock. We had problems with the UART baudrate. When configured to 115200 bits/s we would have about 108000 bits/s. In order to configure Nucleo board to use external crystal (from the STM Link) we have unsoldered SB5 and SB7 solder bridges and soldered SB17 solder bridge. Doing that we have introduced a bug where initial pins PB7 and PB6 for UART did not work, they were in high state when transmitting. To fix the bug we have changed pinout to PA10 and PA9 for the UART1. 
	
## Usage
### Parser
To use parser functionalities:
1. Connect to the bluetooth module on the board with some terminal program. I tend to use the "Serial Bluetooth Terminal 1.36" on my Android phone to send and receive character commands. 
2. Add your callback function definitions and declarations in the `callbacks.hpp` and `callbacks.cpp` files and also add function pointers to the `m_callbacks` variable in `parser.hpp` file.
3. Send commands as `echo(some text)\n`. Be aware that every parser command, in order to be received, must end with the EOL ('\n') character.
4. Observe incomming messages.
