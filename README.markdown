```
  __      _(_)_ __ | |_ ___ _ __ _ __ ___  _   _| |_ ___ 
  \ \ /\ / / | '_ \| __/ _ \ '__| '_ ` _ \| | | | __/ _ \
   \ V  V /| | | | | ||  __/ |  | | | | | | |_| | ||  __/
    \_/\_/ |_|_| |_|\__\___|_|  |_| |_| |_|\__,_|\__\___|
```

> "The future is already here. It's just unevenly distributed." -
> William Gibson, [Neuromancer (1984)][neuromancer].

[![Build Status][travisdevelop_img]][build_stat]
[![Coverage Status][coverallsdevelop_img]][cover_stat]
[![Stories Ready to be Worked On][waffle_ready_img]][waffleio]
[![Stories Being Worked On][waffle_work_img]][waffleio]
[![Throughput Graph][issuegraph]][issuelink]

# Synopsis
Wintermute's a software project that intends to be a launching pad into
self-driven hyper-connectivity of hardware and software. The whole purpose
of this particular tree is to build a core library, a driver binary
(more on that) and a few plugins to get the show started. Check out
[this weblog post][post] to really understand the mission behind Wintermute.

# Building & Contributing
Building, requirement and installation information all live in
[`INSTALL.markdown`][install]. Pointers for contributing on the project are
located in [`CONTRIBUTING.markdown`][contrib]. So far, Wintermute works on a
[few platforms][platforms], but feel free to build for yours and try it out.
This project is [Vagrant][] enabled.

**This is alpha software, so alpha that it does nothing.** But in due time, my
friend.

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
[coverallsdevelop_img]: https://img.shields.io/coveralls/jalcine/wintermute.svg?branch=develop
[waffle_ready_img]: https://badge.waffle.io/jalcine/wintermute.png?label=ready&title=Ready
[waffle_work_img]: https://badge.waffle.io/jalcine/wintermute.png?label=In%20Progress&title=In%20Progress
[issuegraph]: https://graphs.waffle.io/jalcine/wintermute/throughput.svg
[issuelink]: https://waffle.io/jalcine/wintermute/metrics
[contrib]: ./CONTRIBUTING.markdown
[install]: ./INSTALL.markdown
[platforms]: ./INSTALL.markdown#platforms
[vagrant]: https://vagrantup.com
[neuromancer]: http://www.goodreads.com/book/show/22328
