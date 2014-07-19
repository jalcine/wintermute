#include "designation.hh"
#include "designation.hpp"

using Wintermute::Procedure::DesignationPrivate;
using Wintermute::Procedure::Designation;

Designation::Designation(const string& name, const string& domain, const pid_t& pid) : 
  d_ptr (new DesignationPrivate)
{
  W_PRV(Designation);
  d->name = name;
  d->domain = domain;
  d->pid = pid;
}

Designation::Designation() : d_ptr(new DesignationPrivate)
{
  W_PRV(Designation);
  d->name = "";
  d->domain = "";
  d->pid = getpid();
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

bool Designation::isLocal() const
{
  W_PRV(const Designation);
  return d->pid == getpid();
}

bool Designation::isNull() const
{
  W_PRV(const Designation);
  return d->domain.empty() && d->name.empty();
}

Designation::~Designation()
{}
