#include "command_line.hpp"
#include <Wintermute/Testing>
#include <QtTest/QTest>
#include <QtCore/QString>
#include <iostream>
#include <QDebug>

using Wintermute::Testing::spawnProcess;

void
CommandLineUnitTest::cleanup(){
  if (process->state() != QProcess::NotRunning){
    process->close();
  }

  process->deleteLater();
}

void
CommandLineUnitTest::showHelp() {
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
CommandLineUnitTest::showVersion() {
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
CommandLineUnitTest::showInvalidArgument() {
  process = spawnProcess(QStringList() << "--january");

  // Check if the apps runs.
  QVERIFY(process->waitForStarted());
  QVERIFY(process->waitForFinished());

  // Capture output from stderr, as promised.
  QByteArray output = process->readAllStandardOutput();
  QCOMPARE(!output.isEmpty(), true);
  QCOMPARE(!output.isNull(), true);
}

QTEST_MAIN(CommandLineUnitTest)

#include "command_line.moc"
