/**
 * vim: ft=qt.cpp
 *
 */
#include "Wintermute/Procedure/Hosts/base.hpp"

using Wintermute::Procedure::Hosts::Base;

Base::Base() { }

Base* Base::discoverHost( QString& const qualifiedName ) {
  return 0;
}

Base::~Base() { }
