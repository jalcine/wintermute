#!/usr/bin/env sh

# Build qcommandline
# TODO: Add this from a PPA instead
git clone git://github.com/jalcine/qcommandline
cd qcommandline
mkdir build
cd build
cmake .. -DCMAKE_INSTALL_PREFIX=/usr -DCMAKE_BUILD_TYPE=Debug
make
sudo make install
cd ..

# Build log4qt
# TODO: Add this from a PPA instead.
git clone git://gitorious.org/log4qt/log4qt.git
cd log4qt
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=/usr -DCMAKE_BUILD_TYPE=Debug
make
sudo make install
cd ..

git clone git://github.com/zeromq/cppzmq
sudo install cppzmq/zmq.hpp /usr/include/zmq.hpp

# Build QtZeroMQ.
# TODO: Add this from a PPA.
git clone git://github.com/jalcine/QtZeroMQ
cd QtZeroMQ
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Debug -DCMAKE_INSTALL_PREFIX=/usr
make
sudo make install
cd ..

# Install coveralls's command line tool.
sudo pip install cpp-coveralls --use-mirrors
