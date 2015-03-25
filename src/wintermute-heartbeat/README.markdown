# Heartbeats in Wintermute {#heartbeat}

The heartbeat plugin serves as Wintermute's means of bringing awareness to the
neighboring plugin irrespective of the underlying RPC logic used to communicate.
The idea behind this is to allow for the introduction of alternative RPC and
message broker tools without having to do too much of a rewrite of the system of
Wintermute.

## Call Cycle

When a new process starts up, considered a _node_, it sends out a SYN call to
inform the heartbeat instance of its existence. The heartbeat instance, in
return, replies to that call with an ACK call with its heartbeat specific UUID
as well as the UUIDs of its neighboring modules.

SYN call:

```
{
  'modules': ['module1', 'module2', 'module3'],
  'plugins': ['plugin1', 'plugin2'],
}
```

ACK call (empty):
```
{
  'uuid': 'b2c74df9-9c90-4024-a818-4a7ad0cc5d30'
}
```

This cycle continues indefinitely.

## Notes

  * What should a client node do if it notices that its UUID has changed?
    - What kind of assumptions can be made here?
    - Do we classify this as a system restart? If so, why?
  * How do we prevent forgery of message structure by outside sources?
    **TODO**: Look into using libsodium to encrypt all messages.

---
[^1]: <https://en.wikipedia.org/wiki/Heartbeat_%28computing%29>
