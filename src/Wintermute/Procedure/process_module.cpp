#include "Procedure/process_module.hpp"

using Wintermute::Procedure::ProcessModule;

ProcessModule::ProcessModule() : Module(Wintermute::Application()) {
  // TODO: Add 'listModules' method via lambda.
  // TODO: Add 'reboot' method via lambda.
}

QString
ProcessModule::domain() const { return "me.jalcine.wintermute"; }

QString
ProcessModule::package() const { return "process"; }

ProcessModule::~ProcessModule() {
}
