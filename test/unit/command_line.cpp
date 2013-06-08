#include "command_line.hpp"
#include <Wintermute/Testing>
#include <QtTest/QTest>
#include <QtCore/QString>
#include <iostream>
#include <QDebug>

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
  QByteArray output = process->readAllStandardError();
  QCOMPARE(!output.isEmpty(), true);
  QCOMPARE(!output.isNull(),  true);
}

void
TestCommandLine::showVersion() {
  process = Wintermute::Testing::spawnProcess(QStringList() << "--version");

  // Check if the apps runs.
  QVERIFY(process->waitForStarted());
  QVERIFY(process->waitForFinished());

  // Capture output from stderr, as promised.
  QByteArray output = process->readAllStandardError();
  QCOMPARE(!output.isEmpty(), true);
  QCOMPARE(!output.isNull(),  true);

}

void
TestCommandLine::showInvalidArgument() {
  process = spawnProcess(QStringList() << "--january");

  // Check if the apps runs.
  QVERIFY(process->waitForStarted());
  QVERIFY(process->waitForFinished());

  // Capture output from stderr, as promised.
  QByteArray output = process->readAllStandardOutput();
  QCOMPARE(!output.isEmpty(), true);
  QCOMPARE(!output.isNull(), true);
  QVERIFY(output.contains("Malformed"));
}

QTEST_MAIN(TestCommandLine)

#include "command_line.moc"
