apt-get update
apt-get install -y ninja-build cmake gcc-arm-none-eabi gcc cppcheck gdb-multiarch openocd

# add link arm-none-eabi-gdb -> gdb-multiarch
ln -s /usr/bin/gdb-multiarch /usr/bin/arm-none-eabi-gdb
