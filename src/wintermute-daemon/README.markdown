# Daemon Plugin for Wintermute {#daemon}

The daemon plugin in Wintermute serves as a way to maintain plugins that
Wintermute needs in order to stabilize its remote procedure calling system like
module existence on a local machine and sending machines from a local machine
out over a network. It does this by reading the daemon's configuration options
and loading the designation plugins to be used for _relaying messages_ over a
stream (UNIX socket, TCP, etc) and starts that up.

The next step would be loading the **heartbeat** plugin. The heartbeat plugin
collects heartbeat signals from other processes on the local system and allows
one to build a model of what the system looks in an elastic fashion. This model
isn't required for _any_ procedure calling but it's provided as a model for
services that'd like to ensure the existence of another module or call.

After that, the **RPC relay** plugin, designated by the daemon's configuration,
is loaded and prepped to listen for incoming messages from the external network
and send outgoing messages to the outgoing network.

## Heartbeat Plugin {#heartbeat}

The heartbeat plugin is an utility plugin loaded into the daemon instance that
collects information from other instances of Wintermute running on the local
machine. It fetches this by sending out pings designated to be answered by 
instances of Wintermute and collects pongs and records the time of said
collection to see which process is still currently active or is need of a kick.
Very simple in ideology, if you ask me.

## Loading the RPC Relay Plugin {#rpc-relay}

The RPC relay is meant to transport messages designated to go _off_ the current
network or come from _outside_ the current network and pass them back onto the
local network. This is handy for having instances of Wintermute running
on the same network but on different machines, perhaps for home automation or
distributed computing. By default, [ZeroMQ][] is used to handle local _and_
remote communications between Wintermute. The addresses it listens to is
defined with compile-time defaults, `WINTERMUTE_ZMQ_ADDRESS_LOCAL` and
`WINTERMUTE_ZMQ_ADDRESS_REMOTE` but it also allows for added addresses in the
event that the internal defaults don't work on said machine.

[ZeroMQ]: http://zeromq.org/
