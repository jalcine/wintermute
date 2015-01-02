Event Loops         {#events}
=============================

Wintermute makes use of an [event loop][] to handle the queuing and invocation
of tasks, specifically for the `Wintermute::Tunnel` class. This allows for one
to *subscribe* to a specific kind of message (backed by Wintermute::Message
objects) and *send* messages without too much hassle.

The event loop's heavily abstracted from the user and is most visible by the
Wintermute::Tunnel::isActive() function, which determines whether or not is
there an active loop polling for events pertaining to message transversal. 

[event loop]: https://duckduckgo.com/?q=event+loop&t=canonical&ia=about
