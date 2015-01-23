# [ZeroMQ][] Support for Wintermute
This library aims to extend Wintermute's ability to communicate with other
processes by making use of [ZeroMQ][], specifically in a publish-subscribe
fashion. There's a core socket that Wintermute communicates over in a
write-only fashion dubbed `wintermute.sock`. Each sub process has their own
dedicated input socket, dubbed `wintermute-${PID}.sock`. There's also a general
purpose TCP port used by Wintermute to allow for external communication with
all sub processes.

[zeromq]: https://github.com/zeromq/zmqpp
