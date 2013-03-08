#include "command_line.hpp"
#include <QtTest>

QProcess*
TestCommandLine::buildProcess(){
  QProcess* process = new QProcess(this);
}

void
TestCommandLine::passInVersion(){
  QProcess* process = this->buildProcess();
  
}

void
TestCommandLine::passInHelp(){
}

void
TestCommandLine::passInvalidArguments(){
}


QTEST_MAIN(TestCommandLine)

#include "command_line.moc"
