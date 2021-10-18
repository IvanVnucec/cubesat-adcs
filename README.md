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
	- Core/Src/freertos.c - OS tasks
	- Drivers/mpu9250 - MPU9250 IMU drivers

### Code formatting
	1. Install LLVM package https://prereleases.llvm.org/win-snapshots/LLVM-12.0.0-6923b0a7-win64.exe
	2. Install CppStyle 1.5.0.0 on Eclipse Marketplace https://marketplace.eclipse.org/content/cppstyle
	3. Go to Windows - Preferences - C/C++ - CppStyle and 
		- add Clang-format path and
		- check Run clang-format on file save
