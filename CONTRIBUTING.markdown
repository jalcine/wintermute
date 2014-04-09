# Contributing To Wintermute

Firstly, **thanks** for wanting to work on the project! It's a (very) personal
project and for people to come in and help means a *lot*. **Thanks**.

Secondly, you'd need to have a few tools installed on your system. Things like
`gdb` would be exceedingly useful here. Check the corresponding 
[debugging](#Debugging) and [testing](#Testing) sections. As always, feel free
to suggest updates and changes to this and any part of the application and its
libraries. Also, keep an eye on your pull request status when testing code on
the [continuous integration builds over at TravisCI][travisci].

## Setting Up

## Debugging

TODO: Work on using GDB with Wintermute.
TODO: Mention the need to use Qt4 pretty printers with GDB here.

## Testing

Testing in Wintermute is done with the Qt testing library as its driver and
uses CMake to run said tests. The tests are not built after any of the plugins
or core binary is built so you'll have to invoke said test building by hand.

[travisci]: http://travis-ci.org/wintermuteapp/wintermute
