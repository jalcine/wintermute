#!/usr/bin/env sh

alias addpkg="sudo apt-get install -qq"

echo -en "Running tests on $(uname -a)."

# Install pkg-config.
addpkg pkg-config cmake

# Install CxxTest.
addpkg cxxtest

# Install app dependecies.
addpkg libjsoncpp-dev
addpkg liblog4cxx10-dev
addpkg libboost1.54-dev
addpkg libboost-filesystem1.54-dev
addpkg libclang-*-common-dev

echo "Done loading dependencies."
