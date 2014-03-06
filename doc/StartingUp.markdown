# Starting Up Wintermute

The idea here for starting and running Wintermute is meant to be universal
across the (potentially) different language implementations of the software to
allow for a common core concept of the application.

## Cold Start

When Wintermute is invoked from the command line, the very thing to occur is
the initialization of its logging system. It'd send all of its output to disk
and to the current terminal output window. After that, it checks the
designated mode for this instance of Wintermute to run as[^1]. Right now, it's
configured to pick between a **daemon** mode or a **plugin** mode. The daemon
mode is a quasi-alias toward the invocation of the `wintermute-daemon` plugin.

### Starting As the Daemon

The daemon instance of Wintermute has to start the plug-ins designated by the
configured list set at build-time (defined by WINTERMUTE_DAEMON_STARTUP_LIST).

[^1]: The information about modes can be discovered in
      `src/Wintermute/private/application.hpp` in the
      `Wintermute::ApplicationPrivate` in the `loadCurrentMode()` function.
