#ifndef UNIT_TEST_CLEAN_START_HPP
#define UNIT_TEST_CLEAN_START_HPP

#include <QtCore/QObject>

class TestCleanStart : public QObject
{
	Q_OBJECT;

private Q_SLOTS:
	void startApp();
};

#endif /* UNIT_TEST_CLEAN_START_HPP */
