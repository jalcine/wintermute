#ifndef WINTERMUTE_CORE_PRIVATE_PLUGININTERFACEABLE_HPP
#define WINTERMUTE_CORE_PRIVATE_PLUGININTERFACEABLE_HPP

#include <Wintermute/Globals>
#include <QtCore/QObject>
#include "plugininterfaceable.hpp"

namespace Wintermute {
  class PluginInterfaceablePrivate {
    Q_DECLARE_PUBLIC(PluginInterfaceable);
    PluginInterfaceable* q_ptr;

    public:

    // @note Should we connect signals from interface to plugin here?
    PluginInterfaceablePrivate(PluginInterfaceable* q) : q_ptr(q) {
    }
  };
}

#endif
