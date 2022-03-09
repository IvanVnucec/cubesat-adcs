# install vscode
sudo apt-get install wget gpg
wget -qO- https://packages.microsoft.com/keys/microsoft.asc | gpg --dearmor > packages.microsoft.gpg
sudo install -o root -g root -m 644 packages.microsoft.gpg /etc/apt/trusted.gpg.d/
sudo sh -c 'echo "deb [arch=amd64,arm64,armhf signed-by=/etc/apt/trusted.gpg.d/packages.microsoft.gpg] https://packages.microsoft.com/repos/code stable main" > /etc/apt/sources.list.d/vscode.list'
rm -f packages.microsoft.gpg

sudo apt install apt-transport-https
sudo apt update
sudo apt install code

# install cortex-debug extension in vscode 
code --install-extension marus25.cortex-debug

# install other dependencies
sudo apt-get update
sudo apt-get install -y ninja-build cmake gcc-arm-none-eabi gcc cppcheck gdb-multiarch openocd clang-format

# add link arm-none-eabi-gdb -> gdb-multiarch
ln -s /usr/bin/gdb-multiarch /usr/bin/arm-none-eabi-gdb
