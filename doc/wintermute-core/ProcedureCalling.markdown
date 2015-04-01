Procedure Calling   {#procedure-calling}
==================

Wintermute uses a module loosely related to that of what D-Bus uses
(registering objects and methods onto a bus and then having people interact
with a specific bus and its sub-components) to handle its procedure calling
with other entities. The mission is to have the delivery of said messages to
be ambiguous. It'd encompass the following:

  - **Call Registration**: Within each Module, Call objects can be registered
    and named. This allows for Module objects to represent a collected sense
    of function to a particular task.

  - **Method Invocation**: Serving as the glue between a ModuleCall and the RPC
    system, Method objects contain meta data about the arguments that a
    ModuleCall would use, the source of said invokation and the desination of
    said call.

  - **Module Registration**: within each process, modules can be loaded
    that can be listed and found.

  - **Process Registration**: each running process of Wintermute should
    be able to 'expose' itself over a network

  - **Module Exposure**: expose modules within their process

  - **Process Exposure**: expose process over network.

## Procedure Flow
With the following, a hierarchy of the flow would be something like this:

~~~
-- me.jalcine.wintermute.root ( domain.app.nickname )
  |-- * process
  |-- restart()
|-- pid()
  |-- ..
  |-- * master
  |-- reboot()
|-- shutdown()
  |-- ...
  |-- * factory
  |-- allPlugins()
  |-- loadPlugin(plugin_id)
|-- reloadPlugin(plugin_id)
  |-- ...
~~~

A request to a method would be formed like: 
~~~
me.jalcine.wintermute.root?process::restart()
~~~

## Methods
Methods, when defined in Wintermute, would adhere to the following:
  * should be able to *return* a value, be it `null`, a single value or 
    potentially a tuple.

  * can be classified as one of the following:
    * `blocking` - the current thread that called the method will be 
      blocked until the method returns.
    * `async` - the current thread won't be blocked, but a callback 
      method to collect the value will be required to work on it from then.

### Calls
There's three system calls that can be made in Wintermute; the `method` call,
`exist` call and the `signal` calls. The `MethodCall` class manages the work 
of dispatching methods between modules. The purpose of this base class allows 
for a centralized inheritance of other classes that wish to build upon the 
hierarchy of method calling. The `ExistCall` provides a means of determining 
the existence of other entities on the network like modules, methods and 
signals. One can take these base classes further and use them to determine 
the existence of other constructed objects or invoke methods on other entities.

## Notes 
  + Since most of the application is splitting into separate threads 
    (the core thread, the plug-in thread and the procedure thread being the 
    central threads), ensure that we have a per-initialization method 
    that allows us to create the necessary global context for the process.

  + Initially, I was going to use [ZeroMQ][] to handle this. But that 
    library is _very_ opinionated on how things should work although 
    it allows for an arbitrary event loop to be used. Now, the prospect 
    of using something like ZeroMQ is high; the community is strong and 
    the software's tried but I want to experiment using something else. 
    With enough encapsulation of how the event loop would work for 
    Wintermute, it should be (in light terms) easy to swap different 
    event loops to experiment.

[zeromq]: http://zeromq.org
