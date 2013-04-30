#!/usr/bin/env sh

git clone git://github.com/jalcine/qcommandline
cd qcommandline
mkdir build
cd build
cmake .. -DCMAKE_INSTALL_PREFIX=/usr -DCMAKE_BUILD_TYPE=Debug
make
sudo make install


git clone git://gitorious.org/log4qt/log4qt.git
cd log4qt
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=/usr -DCMAKE_BUILD_TYPE=Debug
make
sudo make install
