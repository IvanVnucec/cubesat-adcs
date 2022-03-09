BUILD_FOLDER = build
SCRIPTS_FOLDER = scripts
TEST_FOLDER = tests
CPPCHECK_BUILD_FOLDER = build_cppcheck

.PHONY: all install_deps setup_cmake build test flash clang_format cppcheck help debug_gdb clean

all: build

install_deps:
	bash ./$(SCRIPTS_FOLDER)/install_deps.sh

setup_cmake: 
	rm -rf $(BUILD_FOLDER)
	mkdir $(BUILD_FOLDER)
	cd $(BUILD_FOLDER) && \
	cmake -GNinja -DCMAKE_TOOLCHAIN_FILE=../cmake/CMakeSetCompiler.cmake -DCMAKE_BUILD_TYPE=Debug ..

build:
	cd $(BUILD_FOLDER) && ninja

test:
	cd $(TEST_FOLDER) && \
	cmake -S . -B build && \
	cmake --build build && \
	cd build && \
	ctest

flash:
	cd $(BUILD_FOLDER) && ninja flash

clang_format:
	cd $(BUILD_FOLDER) && ninja clang_format

cppcheck:
	mkdir -p $(CPPCHECK_BUILD_FOLDER)
	cppcheck --template=gcc --cppcheck-build-dir=$(CPPCHECK_BUILD_FOLDER) src

debug_gdb:
	cd $(BUILD_FOLDER) && ninja debug_gdb

clean:
	cd $(BUILD_FOLDER) && ninja clean
	rm -rf $(CPPCHECK_BUILD_FOLDER)
    #cd $(TEST_FOLDER) && ninja clean

help:
	@echo all install_deps setup_cmake build test flash clang_format cppcheck help debug_gdb clean
