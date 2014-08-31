#!/usr/bin/env sh

alias addpkg="sudo apt-get install -qq"

echo -en "Running tests on $(uname -a)."

# Install CMake and pkg-config
addpkg cmake pkg-config

# Install CxxTest.
addpkg cxxtest

# Install app dependecies.
addpkg libjsoncpp-dev
addpkg liblog4cxx10-dev
addpkg libboost1.54-dev
addpkg libboost-filesystem1.54-dev

echo "Done loading dependencies."
