#include "clean_start.hpp"
#include <QtTest/QtTest>
#include <Wintermute/Testing>

using Wintermute::Testing::spawnProcess;

void
TestCleanStart::startApp()
{
	QSKIP ( "Add tests to determine a clean start up for the application.", SkipAll );
}

QTEST_MAIN ( TestCleanStart )

#include "clean_start.moc"
