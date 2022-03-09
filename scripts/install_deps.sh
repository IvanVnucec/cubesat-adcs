#!/bin/bash

sudo apt-get update

# install necessary dependencies
sudo apt-get install -y ninja-build cmake gcc-arm-none-eabi gcc cppcheck

# install other dependencies
if [[ ${GITHUB_ACTIONS=} == "true" ]]; then
    :
else
    sudo apt-get install -y gdb-multiarch openocd clang-format

    # add link arm-none-eabi-gdb -> gdb-multiarch
    ln -s /usr/bin/gdb-multiarch /usr/bin/arm-none-eabi-gdb

    # install vscode
    sudo apt-get install wget gpg
    wget -qO- https://packages.microsoft.com/keys/microsoft.asc | gpg --dearmor > packages.microsoft.gpg
    sudo install -o root -g root -m 644 packages.microsoft.gpg /etc/apt/trusted.gpg.d/
    sudo sh -c 'echo "deb [arch=amd64,arm64,armhf signed-by=/etc/apt/trusted.gpg.d/packages.microsoft.gpg] https://packages.microsoft.com/repos/code stable main" > /etc/apt/sources.list.d/vscode.list'
    rm -f packages.microsoft.gpg

    sudo apt-get install apt-transport-https
    sudo apt-get update
    sudo apt-get install code

    # install vscode extensions 
    LD_LIBRARY_PATH="" LD_PRELOAD="" code --user-data-dir=$HOME/.config/Code/ --extensions-dir=$HOME/.vscode/extensions/ --install-extension marus25.cortex-debug
    LD_LIBRARY_PATH="" LD_PRELOAD="" code --user-data-dir=$HOME/.config/Code/ --extensions-dir=$HOME/.vscode/extensions/ --install-extension ms-vscode.cpptools
fi
