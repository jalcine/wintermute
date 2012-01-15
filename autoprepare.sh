#!/bin/sh
# @author Adrian Borcuki <gentoolx@gmail.com>
# This shell script prepares this modular branch for building by pulling
# the other repositories of Wintermute.

# Base part of Git address on GitHub
GIT_BASE=git://github.com/wntr

# Now, set addresses for every repository that we must pull.
GIT_WDATA=$GIT_BASE/data.git
GIT_WNETWORK=$GIT_BASE/ntwk.git
GIT_WLING=$GIT_BASE/ling.git
GIT_WPLUGINS=$GIT_BASE/plugins.git

# Having these set we can start preparing.
# First of all, we pull needed branches.
echo "*** Cloning $GIT_WDATA..."
git clone $GIT_WDATA lib/wntrdata

echo "*** Cloning $GIT_WNETWORK..."
git clone $GIT_WNETWORK lib/wntrntwk

echo "*** Cloning $GIT_WLING..."
git clone $GIT_WLING lib/wntrling

echo "*** Cloning $GIT_WPLUGINS..."
git clone $GIT_WPLUGINS lib/plugins

# Ok, let's use Sed to correct some files such that everything works.
echo "*** Making file customizations..."
sed '/#$/ {
     N
     /# Installs/ iadd_subdirectory(lib)
    }
    ' CMakeLists.txt > CMakeLists.new

cat > lib/CMakeLists.txt <<EOF
cmake_minimum_required(VERSION 2.8)
add_subdirectory(wntrdata)
add_subdirectory(wntrntwk)
add_subdirectory(wntrling)
add_subdirectory(plugins)
EOF

case "$1" in
  "enable-patches")
    diff -U 3 -dN -- CMakeLists.txt CMakeLists.new > CMakeLists.txt.patch
    rm CMakeLists.new
    ;;
  *)
    mv CMakeLists.new CMakeLists.txt
    ;;
esac

mkdir -p build
cd build
cmake -DCMAKE_INSTALL_PREFIX=/usr -DCMAKE_BUILD_TYPE=Debug
make
sudo make install
