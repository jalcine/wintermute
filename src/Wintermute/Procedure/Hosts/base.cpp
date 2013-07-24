/**
 * vim: ft=qt.cpp
 *
 */
#include "Wintermute/Procedure/Hosts/base.hpp"
#include "Wintermute/Procedure/Agents/base.hpp"
#include "Wintermute/private/Procedure/Hosts/base.hpp"
#include "Wintermute/logging.hpp"

using namespace Wintermute::Procedure;

Hosts::Base::Base() { }

// NOTE: Very functional!
Hosts::Base* Hosts::Base::discoverHost( QString& qualifiedName ) {
  return Agents::Base::pingHost(qualifiedName, Agents::Base::PingQuickestHost).first();
}

bool Hosts::Base::dispatchCall( Call* const call ) {
  // TODO: Inform network of impending call.
  // TODO: Obtain the host to be called upon.
  // TODO: Serialize the arguments of the call.
  // TODO: Send call to all agents.
  return false;
}

Hosts::Base::~Base() { 
}
