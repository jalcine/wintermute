#!/usr/bin/env sh

alias cm="mkdir -p build && cd build && cmake .. -DCMAKE_BUILD_TYPE=Debug -DCMAKE_INSTALL_PREFIX=/usr; cd .."
alias db="make -C build all && sudo make -C build install; cd .."

git clone git://gitorious.org/qcommandline/qcommandline.git ~/qcommandline
git clone git://gitorious.org/log4qt/log4qt.git ~/log4qt

cd ~/qcommandline && cm && db
cd ~/log4qt && cm && db

#git clone git://github.com/zeromq/cppzmq ~/cppzmq
#sudo install ~/cppzmq/zmq.hpp /usr/include/zmq.hpp
#git clone git://github.com/jalcine/QtZeroMQ ~/QtZeroMQ --branch develop
#cd ~/QtZeroMQ && cm && db

echo "Done loading dependencies."
