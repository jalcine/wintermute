# Procedure Calling
The act of procedure calling in Wintermute allows for a perfect opporunity to 
implement a concrete means of interally extending Wintermute. Since Wintermute 
doesn't attempt to stick to a single platform standard, we moved from the 
original implementation of D-Bus to ZeroMQ. Wintermute uses **hosts** to 
collect a suite of methods whom of which contain **methods** that can be 
invoked. The term **hosts** and **objects** are used interchangably.

## Hosts
**Hosts** give Wintermute the ability to define a set of functionality and 
serve it out to other instances of Wintermute without too much fuss on how 
it'd discover itself in the world. The base class for hosts is 
`Wintermute::Procedure::Hosts::Base`. It provides the methods required to 
become a host but nothing more. Using interfaces, Wintermute allows developers 
to mold their hosts to whatever situation that they see fit.

For most cases, using the `Wintermute::Procedure::Hosts::Local` would be 
enough for developers that wish to communicate with Wintermute instances 
on a local machine and using `Wintermute::Procedure::Hosts::Remote` would
provide their host with the capability of speaking to remote instances of
Wintermute. These two are based on the versions of IPC / RPC Wintermute
recommends, that being ZeroMQ for local and remote socket communication.

Now, some developers may wish to further extend their support of Wintermute to 
different protocols like via JSON-RPC, D-Bus or even via an HTTP API. Work for 
said plug-ins are looked towards in the future but would NOT be bundled by 
default into Wintermute, but rather in the Wintermute networking plug-in.

## Methods
Internally, methods are the invocable functions that a host exposes to another 
host. Wintermute uses the QVariant system to convert QVariant-compatible 
objects into QString and then use said raw value over the specified protocol. 
In code, you'd typically invoke calls on a hosts in two ways; using the 
`Wintermute::Procedure::Call` object or by using the interface named.
`Wintermute::Procedure::Invokable` provides methods and logic for `QObject`s
to get the capability required to handle Call events.

### `Wintermute::Procedure::Call`
The `Call` object gives you the ability to invoke a call manually with all of 
the options available to you. It's the more tedious route.

*TODO*: Add code sample.
