#include "clean_start.hpp"
#include <QtTest/QtTest>
#include <Wintermute/Testing>

using Wintermute::Testing::spawnProcess;

void
TestCleanStart::startApp(){
  QProcess* process = spawnProcess(QStringList());
  QVERIFY(process->waitForStarted());
  process->close();
}

QTEST_MAIN(TestCleanStart)

#include "clean_start.moc"
