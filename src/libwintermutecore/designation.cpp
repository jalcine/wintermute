#include "designation.hh"
#include "designation.hpp"

using Wintermute::Procedure::DesignationPrivate;
using Wintermute::Procedure::Designation;

Designation::Designation() : d_ptr (new DesignationPrivate)
{
}

Designation::Designation(const Designation& other) : d_ptr(new DesignationPrivate)
{
  W_PRV(Designation);
  d->clone(other.d_ptr);
}

string Designation::name() const
{
  W_PRV(const Designation);
  return d->name;
}

string Designation::domain() const
{
  W_PRV(const Designation);
  return d->domain;
}

pid_t Designation::pid() const
{
  W_PRV(const Designation);
  return d->pid;
}

Designation::~Designation()
{}
