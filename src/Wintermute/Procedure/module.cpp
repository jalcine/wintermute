#include "Wintermute/Procedure/module.hpp"
#include "Wintermute/private/Procedure/module.hpp"

using Wintermute::Procedure::Module;
using Wintermute::Procedure::ModulePrivate;

ModulePrivate::ModulePrivate(Module* q) : q_ptr(q) {
  // TODO: Grab a pointer to the shared instanec of ZeroMQ here.
  // TODO: Set up a client/server socket between the ports of '3991' to '4000'.
}

ModulePrivate::~ModulePrivate() {
}

Module::Module(QObject* parent) : QObject(parent), d_ptr(new ModulePrivate(this)){
}

QString Module::qualifiedName() const {
  return QString::null;
}

Module::~Module() {
  // TODO: Report to world that you're leaving us.
  // TODO: Disconnect sockets.
}

#include "Wintermute/Procedure/module.moc"
