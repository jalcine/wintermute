#include "module_designation.hh"
#include "module.hpp"

using Wintermute::Module;
using Wintermute::Module;
using Wintermute::ModulePrivate;
using Wintermute::DesignationPrivate;

Module::Module::Designation::Designation(const string& name, const string& domain, const pid_t& pid) :
  d_ptr (new DesignationPrivate)
{
  W_PRV(Designation);
  d->name = name;
  d->domain = domain;
  d->pid = pid;
}

Module::Designation::Designation(const Designation& other) : d_ptr(new DesignationPrivate)
{
  W_PRV(Designation);
  d->clone(other.d_ptr);
}

string Module::Designation::name() const
{
  W_PRV(const Designation);
  return d->name;
}

string Module::Designation::domain() const
{
  W_PRV(const Designation);
  return d->domain;
}

pid_t Module::Designation::pid() const
{
  W_PRV(const Designation);
  return d->pid;
}

bool Module::Designation::isLocal() const
{
  W_PRV(const Designation);
  return d->pid == getpid();
}

bool Module::Designation::isNull() const
{
  W_PRV(const Designation);
  return d->domain.empty() && d->name.empty();
}

Module::Designation::operator string() const
{
  return string( domain() + "." + name() + ":" + std::to_string(pid()) );
}

bool Module::Designation::operator==(const Designation& other) const
{
  return other.name() == name() &&
         other.domain() == domain() &&
         other.pid () == pid();
}

Module::Designation::~Designation()
{
}

void DesignationPrivate::clone(const SharedPtr<DesignationPrivate>& other)
{
  this->pid = other->pid;
  this->name = other->name;
  this->domain = other->domain;
}
