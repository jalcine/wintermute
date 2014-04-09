# Procedure Calling

Wintermute abstracts its IPC system with the classes found under the 
`Wintermute::Procedure` namespace. Under the hood, Wintermute isn't quite sure
*how* the call from one process to another gets there but Wintermute does the
work necessary to inform invokers of the arrival of a call as well as
returning a reply once handled.

## Calls

Call objects are the representation of action and data that's shuffled back
and forth over a communication protocol. They serve two purposes, to list the
valid calls available on a **Module** and to hold the proper information
required for invocation on a remote instance of Wintermute. The implementation
for calls are a bit tricky. They can be serialized into a format usable to
send over the wire, yet these functors are required to handle invocation to
begin with. Think of them as a data type with an invokable nature.

## Modules

Modules are objects that hold a collection of calls under a specific namespace. 
Multiple modules can be instantiated in the same process and in multiple
processes at the same time; allowing for common logic to be shared in said
processes. An example of this would be the `ProcessModule`; which would allow
for remote control of the local process. Another would be the modules provided
by the transports used for dispatching and receiving calls like `ZeroMQ::Module`.

## Messaging

Wintermute sends all messages using the Dispatcher system and receives using
the Receiver system. The process is straightforward. When a call is set to be
dispatched, it's serialized into a format appropriate for messaging and then
sent over all of the available dispatchers to be caught by the proper process.
Receivers then listen for these calls and then 
