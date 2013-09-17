# Plug-ins
The nature of Wintermute is *to extend*. Even the daemon instance is run by
loading `wintermute-daemon`. A few other plug-ins that go to extend Wintermute
to the next level are listed below. The plug-ins listed below are proposed
ideas and are in no way conclusive.

## Networking
These plug-ins seek out extending Wintermute's ability to communicate over a
network. By default, Wintermute is restricted to a local machine.

  + `wintermute-tcpip`: This plug-in handles the work to push Wintermute
    over TCP sockets by means of relay. By keeping itself aware of what's
    available on the local machine, it avoids duplicity in message relay.

  + `wintermute-http-json`: This plug-in handles the work of emulating
    Wintermute's IPC by means of a JSON API in the nature of ones you would
    use for Web services.

## Information
Wintermute finds data to be its ally when it comes to find it. By default,
`wintermute-data` gives *data sources* a nifty means of aggregation for other
services that wish to use it. It attempts to take things further by making the
retrieved data semantically passable.

  + `wintermute-data`: This plug-in provides the interfaces and necessary
    foundation of representing a common level of data retrieval, storage and
    editing in Wintermute. It also has a tool that handles auto summarization
    of provided text, attempts to implement relevant and related bits of data
    and a extensible caching system.

  + `wintermute-data-mediawiki`: This plug-in communicates with multiple sites
    that have a working instance of MediaWiki and renders said information for
    Wintermute's data usage. Sites like Wikipedia, WikiMedia, Wikitionary and
    others are meant to be available in its repository listings by default.

  + `wintermute-data-wolframalpha`: Pulls information in a semantic manner
    from Wolfram Alpha's on-line corpora.
  + `wintermute-data-cache-mysql` (#2 likely to be implemented)
  + `wintermute-data-cache-mongo` (most likely to be implemented)
  + `wintermute-data-cache-postgres` (least likely to be implemented)
  + `wintermute-sematics`: A more front-facing plug-in, it handles the act of
    collecting data and parsing results into an ontological format that
    Wintermute can then use to its own means. Another important function is
    its ability to translate to/from a linguistic graph of text from/to an
    ontological graph of data.
  + `wintermute-reason`: Another phase in Wintermute's semantical/ontological
    jumps, the goal of this plug-in is to take the act of a premise of any
    semantically crafted sort and with a set of data, derive a series of
    results. This is what will power Wintermute's ability to respond and
    react.

## Communication
A tipping and perhaps chilling factor in Wintermute's evolution will be its
means of communication. This'll be decided by the plug-ins listed below.

  + `wintermute-linguistics`: This plug-in will facilitate the necessary logic
    to handle natural language processing and communicate heavily with
    `wintermute-sematics` for proper semantics and context and
    `wintermute-data` to pull the necessary grammar and extra linguistics data.
  + `wintermute-voice`: This will handle the act of speech recognition and
    synthesis in Wintermute. Using the appropriate solution for specific
    environments, it'd allow users to directly listen and speak with
    Wintermute.
  + `wintermute-vision`: This will allow to visually be aware of its
    environment. This can range from capturing camera information or infrared
    sensors and then later adopting them to run object recognition and
    autonomous image adaption for visual responses.

# Nature of Plug-ins

There's a few types of plug-ins in Wintermute. **Service plug-ins** are meant
to be light plug-ins that load themselves in every local running instance of
Wintermute. **Instance plug-ins** are meant to be loaded in a separate process
of Wintermute whereas **worker plug-ins** are loaded within an instance
plug-in's process.

## Service Plug-ins

These plug-ins provide auxiliary functionality for the Wintermute platform
like a hot-fix or a tweak to have Wintermute work in a more interesting
fashion. A well-known service plug-in is `wintermute-service-heartbeat`.
It's the plug-in that sends information about processes back to
`wintermute-daemon`'s `HeartbeatModule` for information about the available
modules and processes on the local machine. It's a very useful part of
Wintermute and provides plug-ins with a bit of awareness of their environment.
See [its README][./lib/heartbeat-service/README.md] for more information.

> At times, these plug-ins are called `slices`.

## Instance Plug-ins

Plug-ins of these sort work to provide or improve a new service in Wintermute.
These plug-ins are called `shards` at times. An example of a instance plug-in
that provides a new service would be `wintermute-data` and its providing of
data management and retrieval into Wintermute. See its README for more info.
A instance plug-in that improves a service would be `wintermute-tcp-relay`.

It re-routes most messages (all but ones that's blacklisted or selected only
from a whitelist) on a local machine out of a TCP socket and listens and repeats
incoming messages from the same socket. Such work allows for remote
interfacing with Wintermute over local and remote networks.

## Worker Plug-ins

Worker plug-ins are meant to fill in holes that instance plug-ins leave open
in their implementation. An example of this would be a object relational
mapping (ORM plug-in) that only provides the specification of what a resource
mapping would be like, defined in a instance plug-in. A worker plug-in would
provide the ability to read information from MySQL or MongoDB.
