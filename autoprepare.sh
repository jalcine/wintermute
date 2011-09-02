#!/bin/sh
# This shell script prepares this modular branch for building by pulling 
# needed sources.

# Base part of Git adress on GitHub
GIT_BASE=git://github.com/wntr

# Now, set addresses for every repository that we must pull.
GIT_WDATA=$GIT_BASE/data.git
GIT_WNETWORK=$GIT_BASE/ntwk.git
GIT_WLING=$GIT_BASE/ling.git

# Having these set we can start preparing.
# First of all, we pull needed branches.
# TODO: use a loop here
echo "*** Cloning $GIT_WDATA..."
git clone $GIT_WDATA lib/wntrdata

echo "*** Cloning $GIT_WNETWORK..."
git clone $GIT_WNETWORK lib/wntrntwk

echo "*** Cloning $GIT_WLING..."
git clone $GIT_WLING lib/wntrling

# Ok, let's use Sed to correct some files such that everything works.
echo "*** Making file customizations..."
sed '/#$/ {
     N
     /# Installs/ iadd_subdirectory(lib)
    }
    ' CMakeLists.txt > CMakeLists.new

case "$1" in
  "enable-patches")
    diff -U 3 -dN -- CMakeLists.txt CMakeLists.new > CMakeLists.patch
    rm CMakeLists.new
    ;;
  *)
    mv CMakeLists.new CMakeLists.txt
    ;;
esac
