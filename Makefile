BUILD_FOLDER = build
SCRIPTS_FOLDER = scripts

.PHONY: all install_deps setup_cmake build flash clang_format help debug_gdb clean

all: build

install_deps:
	sh ./$(SCRIPTS_FOLDER)/install_deps.sh

setup_cmake: 
	mkdir $(BUILD_FOLDER)
	cd $(BUILD_FOLDER) && cmake -GNinja -DCMAKE_TOOLCHAIN_FILE=../cmake/CMakeSetCompiler.cmake -DCMAKE_BUILD_TYPE=Debug ..

build:
	cd $(BUILD_FOLDER) && ninja

flash:
	cd $(BUILD_FOLDER) && ninja 

clang_format:
	cd $(BUILD_FOLDER) && ninja clang_format

debug_gdb:
	cd $(BUILD_FOLDER) && ninja debug_gdb

clean:
	cd $(BUILD_FOLDER) && ninja clean

help:
	@echo all setup build flash clang_format help debug_gdb clean
