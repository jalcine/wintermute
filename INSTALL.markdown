# Installing Wintermute

In order to build Wintermute, you'll need the following libraries and programs
already existing on your machine:

  * [boost-filesystem][] 1.5.x
  * [boost-system][] 1.5.x
  * [json-cpp][] 0.6.0
  * [log4cxx] 0.10.0

For testing, you'll need a few more dependencies. Check the [testing
prerequisites](./TESTING.markdown#install) about that.

## Platforms

Wintermute's been built on the following platforms:

Compiler (Version) | Target
:-----------------:|:----------:
GCC 4.9            | amd64-linux
Clang 3.6          | amd64-linux

If you get the tests passing on your platform, please add your platform to the
list above.

## Using Vagrant

If you don't want to install anything on your machine, you can use [Vagrant][]
to run a virtual machine of Ubuntu 14.04 64-bit (the target operating system
for Wintermute) and develop on that from there. Running the following should
get you going:

```bash
$ vagrant up
... vagrant install chef/ubuntu-14.04 ...
... vagrant provision
$ vagrant ssh
# Go into the project's directory.
cd /vagrant
# Run the variant of the test bootstrap for Vagrant.
/vagrant/test/bootstrap --generate-vagrant

[boost-filesystem]: http://www.boost.org/doc/libs/1_55_0/libs/filesystem/doc/index.htm
[boost-system]: http://www.boost.org/doc/libs/1_55_0/libs/system/doc/index.html
[json-cpp]: https://github.com/open-source-parsers/jsoncpp
[log4cxx]: http://logging.apache.org/log4cxx/index.html
[vagrant]: https://vagrantup.com
