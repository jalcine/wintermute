/**
 * vim: ft=qt.cpp
 *
 */
#include "Wintermute/Procedure/Hosts/base.hpp"
#include "private/Procedure/Hosts/base.hpp"
#include "private/vendor/zeromq.hpp"

using Wintermute::Procedure::Hosts::Base;

Base::Base() { }

Base* Base::discoverHost( QString& const qualifiedName ) {
  // TODO: Query network for existence of said host.
  // TODO: Invoke the `selfResolve()` method on host and call data.
  // TODO: Use returned data to form a Hosts::Base* object.
  // TODO: Return if valid.
  return 0;
}

bool Base::dispatchCall( Call* const call ) {
  // TODO: Obtain the host to be called upon.
  // TODO: Serialize the arguments of the call.
  //
  return false;
}

Base::~Base() { }
