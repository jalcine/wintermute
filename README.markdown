# Wintermute [![Stories in Ready][waffleimg]][wafflesrc] [![Build Status][travisimg]][travissrc] [![Gitter chat][gitterimg]][gittersrc] [![Coverage Status][coverageimg]][coverage]

**Wintermute** is meant to be an application platform that aims to:
  + be extended by means of plug-ins
  + embrace the mannerisms of parallel computing when applicable
  + work on desktops, mobile devices and embedded platforms
  + increases its functionality on demand

This repository has been my tinkering to get the application as close to
ideally fit as possibly capable by [me](http://jalcine.me).

Building and/or installing? **Easy**. We use [CMake](http://cmake.org); we
gotcha; read [INSTALL.md](./INSTALL.md) for more information. Feel free to
[contribute](./CONTRIBUTING.md) to the project.

> **Wintermute** itself is built using [Qt][qt4].
> It also uses a few C++11 features like lambdas so it's recommended that you
> build with either `clang >= 3.5-1~exp1` or `gcc >= 4.8.1`.

## Tested Operating Systems
So far, Wintermute has only been built and tested on a Linux operating system,
more precisely, running [Kubuntu][] Raring Ringtail 13.04. Please build, test
and upload your results for comparison!

**Update**: Also builds fine on [Kubuntu][] Saucy Salamander 13.10.

# Contributing

 - Fork, make a topic branch and propose your changes.
   **Bonus points for using Waffle and/or topic branches!**
 - Build and test Wintermute on a foreign, untested system.
 - Spread the word and do your part to make Wintermute an awesome app
   framework.

I *strongly* recommend using tools like [git-flow][gitflow] and
[git-hooks][githooks] to accelerate your development process on
the source control side.

Also, if you're adding a new feature, try writing a test for it first!
Wintermute has a few wrappers over Qt's testing tools. If you get stuck,
copy one of the existing tests and tweak it to your needs. The test suite is
still young so feel free to introduce new (but non-breaking) concepts.

# Support

I (the project maintainer, Jacky Alciné) do my best to answer any and all
questions pertaining to Wintermute. You can e-mail me at me@jalcine.me or
find me on IRC on Freenode, OFTC or the Perl servers with the nick
`jalcine` or `Jacky`. You can also use [Gitter][] to chat about the project.

# Licensing

Wintermute is licensed under the GPLv3 license. See the [license](./LICENSE)
for more details.

[Kubuntu]: http://kubuntu.org
[gitflow]: https://github.com/nvie/gitflow
[githooks]: https://github.com/icefox/git-hooks
[qt4]: http://qt-project.org
[gitter]: https://gitter.im/wintermuteapp/wintermute
[waffleimg]: https://badge.waffle.io/wintermuteapp/wintermute.png
[wafflesrc]: http://waffle.io/wintermuteapp/wintermute
[travisimg]: https://travis-ci.org/wintermuteapp/wintermute.png
[travissrc]: https://travis-ci.org/wintermuteapp/wintermute
[gitterimg]: https://badges.gitter.im/wintermuteapp/wintermute.png
[gittersrc]: https://gitter.im/wintermuteapp/wintermute
[coverageimg]: https://coveralls.io/repos/wintermuteapp/wintermute/badge.png
[coverage]: https://coveralls.io/r/wintermuteapp/wintermute
