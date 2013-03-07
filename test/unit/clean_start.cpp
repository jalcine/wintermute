#include "clean_start.hpp"
#include <QtTest>

void TestQString::toUpper()
{
    QString str = "Hello";
    QVERIFY(str.toUpper() == "HELLO");
}

QTEST_MAIN(TestQString)

#include "clean_start.moc"
