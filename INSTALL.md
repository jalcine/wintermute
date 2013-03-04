# Installing Wintermute

This file documents the process of installing Wintermute onto your operating 
system. These instructions apply to Linux, more specificially Ubuntu Raring 
Ringtail 13.04.

## Requirements

Wintermute requires the following:

+ **CMake** 2.8.4 or greater
+ **Qt** 4.8.4
  + [qcommandline 0.3.0](https://gitorious.org/qcommandline/qcommandline)

## Procedure

```bash
$ mkdir build && cd build              # Create and enter build directory.
$ cmake .. -DCMAKE_BUILD_TYPE=Debug    # Configure Wintermute for debugging.
$ make                                 # Build Wintermute.
$ make test                            # (optional) Test Wintermute.
$ make install                         # Install Wintermute if 
                                       # CMAKE_INSTALL_PREFIX is user writable. 
# make install                         # Install Wintermute system-wide.
