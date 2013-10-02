# Procedure Calling

Wintermute abstracts its IPC system with the `Wintermute::Procedure` classes.
Under the hood, the primary means of RPC is implemented by [ZeroMQ][] and
provided by the `wintermute-tcpip` library that works to implement the network
powered solutions for RPC provided by ZeroMQ (primarily `tcp`). The system is
split up into calls, modules, dispatchers and receivers.

## Calls

Call objects are the representation of action and data that's shuffled back
and forth over a communication protocol. They serve two purposes, to list the
valid calls available on a `Module` and to hold the proper information
required for invocation on a remote instance of Wintermute.

## Modules

Modules are objects that hold a collection of methods under a specific
name space, i.e: 'me.jalcine.wintermute.process' or
'me.jalcine.wintermute.daemon'.
