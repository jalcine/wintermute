# D-Bus Support in Wintermute

This plugin for Wintermute allows each instance to expose itself over
[D-Bus][dbus]; a binary messaging protocol between applications used on Linux.
This can be used in place or in conjunction to other transports like
[ZeroMQ](../zeromq/README.md) to allow for local applications a means of
communication with Wintermute. This is a substitute until a helper library for
external application communication into Wintermute's network can be devised.
