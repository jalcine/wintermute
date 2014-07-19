#!/usr/bin/env sh

alias cm="mkdir -p build && cd build && cmake .. -DCMAKE_BUILD_TYPE=Debug -DCMAKE_INSTALL_PREFIX=/usr; cd .."
alias db="make -C build all && sudo make -C build install; cd .."

echo "Done loading dependencies."
