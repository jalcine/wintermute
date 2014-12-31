A Library As the Core  {#libascore}
====================

<!-- TODO: Define the two projects build here -->
Wintermute's internals are defined in `libwintermutecore`, a C++ library meant
to be built against in possibly different languages but also allowing for
immediate reloads when breaking changes come to play. A shell application
called `wintermute-driver` serves the purpose of loading and starting a plug-in
as a standalone process. This allows for wrapping shell scripts to run and load
specific plugins.
