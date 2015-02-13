# Configuration {#configuration}

Wintermute's configuration is stored using [libconfig][]; providing a level of
flexibility in how the information can be stored. The following are paths at
which Wintermute expects to find said configuration.

  * `/etc/wintermute` - System-wide globals; sanity reference point.
  * `$HOME/.config/wintermute` - User-specific overrides.

For points of reference, all of the locations in which configuration is meant
to be stored is done in accordance to the [XDG Base Directory Specification][xbds].

Wintermute defines an abstraction for configuration loading and parsing via the
`Wintermute::Util::Configuration` class. Check the class documentation header
for more information.

[libconfig]: http://www.hyperrealm.com/libconfig
[xbds]: http://standards.freedesktop.org/basedir-spec/basedir-spec-latest.html
