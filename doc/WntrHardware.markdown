Using the RPi as a interfacing hub into the home network. It'd make sense to
have one Pi per room as we can name them and have them handle core processing.

It would also make sense to get a private build server for Wintermute using
sources lists for Debian so that we can push updates using APT instead of using
a hand-rolled solution.

`zordon.jalcine.me` serves as a public, unrelated server. But if it's possible
to have be connected to a VPN, it can probably serve as a middleman to my VPNs.
Else, we'd have `vpn.jalcine.me` and that'd be a proxying box to whichever VPN
I have set up for that day.

# Hubs

A **hub** serves as a computer that's meant to run an instance of Wintermute
and specialized plugins for the sake of providing extra metadata and relevancy
to Wintermute. Using the bookbag could allow for pedometers, location awareness
and even unlocking when on premises.

Each RPi could be named after a bird picked randomly off a list of the ones
picked out of that book you bought. One for your bedroom, one for the living
room and another to place in the bookbag.

 | Nickname   |  Internal Hostname       |
 | bedroom    |  bedroom.hub.rpi-jalcine |
 | bookbag    |  bookbag.hub.rpi-jalcine |

The bookbag RPI can be sewed on with a denim casing, inside a hard shell to
protect itself. The only concern from there would be battery power, that of
which we can do with a external battery pack from the RPi but connected via USB
into the inside pocket, so it's near seamless. It'd be nice to have an onboard
battery but we'll make do for now.

# Software

We'll have to get the C++ library down and then ported to the RPi architecture,
as well as Android 4.4's Linux. Bindings to Node and Ruby are required to make
scripting Wintermute tremendously easier; using pry as the base REPL for
Wintermute is a +1 as well.
