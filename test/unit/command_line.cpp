#include "command_line.hpp"
#include <Wintermute/Testing>
#include <QtTest/QtTest>

void
TestCommandLine::showHelp() {}

void
TestCommandLine::showVersion() {}

void
TestCommandLine::showInvalidArgument() {}

QTEST_MAIN(TestCommandLine)

#include "command_line.moc"
