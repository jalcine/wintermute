```
  __      _(_)_ __ | |_ ___ _ __ _ __ ___  _   _| |_ ___
  \ \ /\ / / | '_ \| __/ _ \ '__| '_ ` _ \| | | | __/ _ \
   \ V  V /| | | | | ||  __/ |  | | | | | | |_| | ||  __/
    \_/\_/ |_|_| |_|\__\___|_|  |_| |_| |_|\__,_|\__\___|
```

> "The future is already here. It's just unevenly distributed." -
> William Gibson, [Neuromancer (1984)][neuromancer].

**Progress**:

[![Stories Ready to be Worked On][waffle_ready_img]][waffleio] [![Stories Being Worked On][waffle_work_img]][waffleio]

[![Throughput Graph][issuegraph]][issuelink]

**Coverage & Build**:

`develop`: [![Build Status][travisdevelop_img]][build_stat] [![Coverage Status][coverallsdevelop_img]][cover_stat]

`master`:  [![Build Status][travismaster_img]][build_stat] [![Coverage Status][coverallsmaster_img]][cover_stat]

# Synopsis
Wintermute's a software project inspired by a few books, movies and projects
that I (Jacky Alciné) aim to have as an actor for multiple devices and
platforms. I've written about [my ambitions for Wintermute][post] to give an
grandiose vision to the project.

# Building & Contributing
Building, requirement and installation information all live in
[`INSTALL.markdown`][install]. Pointers for contributing on the project are
located in [`CONTRIBUTING.markdown`][contrib]. So far, Wintermute works on a
[few platforms][platforms], but feel free to build for yours and try it out.
This project is [Vagrant][] enabled.

**This is alpha software, so alpha that it does nothing.** But in due time, my
friend.

# Platform Support

Currently, this is being built and tested on Ubuntu 16.04 64-bit. I haven't a
plan to target other platforms as of yet but this is intended to have support
for:

  * Raspberry Pi Debian
  * Android 5.0
  * vanilla Debian 8

# TODO

  + [ ] abstracted configuration framework
  + [ ] preliminary RPC framework (using modules + calls)
  + [x] plugin loading infrastructure
  + [x] versioning support

[post]: http://jalcine.me/weblog/ambitions-wintermute/
[build_stat]: https://travis-ci.org/jalcine/wintermute?branch=develop
[cover_stat]: https://coveralls.io/r/jalcine/wintermute?branch=develop
[waffleio]: http://waffle.io/jalcine/wintermute
[travisdevelop_img]: https://travis-ci.org/jalcine/wintermute.svg?branch=develop
[travismaster_img]: https://travis-ci.org/jalcine/wintermute.svg?branch=master
[coverallsdevelop_img]: https://coveralls.io/repos/jalcine/wintermute/badge.svg?branch=develop
[coverallsmaster_img]: https://coveralls.io/repos/jalcine/wintermute/badge.svg?branch=master
[waffle_ready_img]: https://badge.waffle.io/jalcine/wintermute.png?label=ready&title=Ready
[waffle_work_img]: https://badge.waffle.io/jalcine/wintermute.png?label=In%20Progress&title=In%20Progress
[issuegraph]: https://graphs.waffle.io/jalcine/wintermute/throughput.svg
[issuelink]: https://waffle.io/jalcine/wintermute/metrics
[contrib]: ./CONTRIBUTING.markdown
[install]: ./INSTALL.markdown
[platforms]: ./INSTALL.markdown#platforms
[vagrant]: https://vagrantup.com
[neuromancer]: http://www.goodreads.com/book/show/22328
