#!/bin/sh
# This file creates user and group of name 'wintermute' to allow Wintermute access its internals
# located under directories not belonging to the user running.
# Options: use -m to create /var/lib/wintermute directory

echo "*** Creating user and group of name 'wintermute'... ***"
if [ -e /usr/bin/sudo ]; then
  sudo useradd -r -d /var/lib/wintermute -s /bin/false wintermute "$1"
else
  su -c "useradd -r -d /var/lib/wintermute -s /bin/false wintermute $1"
fi
