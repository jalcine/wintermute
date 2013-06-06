#include "command_line.hpp"
#include <Wintermute/Testing>
#include <QtTest/QTest>
#include <iostream>

using Wintermute::Testing::spawnProcess;

void
TestCommandLine::cleanup(){
  if (process->state() != QProcess::NotRunning){
    process->waitForFinished();
  }

  process->deleteLater();
}

void
TestCommandLine::showHelp() {
  process = spawnProcess(QStringList() << "--help");

  // Check if the apps runs.
  QVERIFY(process->waitForStarted());
  QVERIFY(process->waitForFinished());

  // Capture output from stderr, as promised.
  QByteArray output = process->readAllStandardOutput();
  std::cerr << qPrintable(output);
  QCOMPARE(output.isEmpty(), false);
  QCOMPARE(output.isNull(), false);

  // Check if the apps exits.
  QVERIFY(process->waitForFinished());
}

void
TestCommandLine::showVersion() {
  process = Wintermute::Testing::spawnProcess(QStringList() << "--version");

  // Check if the apps runs.
  QVERIFY(process->waitForStarted());

  // Capture output from stderr, as promised.
  QByteArray output = process->readAllStandardError();
  QCOMPARE(output.isEmpty(), false);
  QCOMPARE(output.isNull(), false);

  // Check if the apps exits.
  QVERIFY(process->waitForFinished());

}

void
TestCommandLine::showInvalidArgument() {
  process = spawnProcess(QStringList() << "--january");

  // Check if the apps runs.
  QVERIFY(process->waitForStarted());

  // Capture output from stderr, as promised.
  QByteArray output = process->readAllStandardError();
  QCOMPARE(output.isEmpty(), false);
  QCOMPARE(output.isNull(), false);

  // Check if the apps exits.
  QVERIFY(process->waitForFinished());
}

QTEST_MAIN(TestCommandLine)

#include "command_line.moc"
