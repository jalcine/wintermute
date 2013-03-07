#ifndef UNIT_TEST_CLEAN_START_HPP
#define UNIT_TEST_CLEAN_START_HPP

#include <QtCore/QObject>

class TestQString : public QObject {
  Q_OBJECT

  private slots:
  void toUpper();
};

#endif /* UNIT_TEST_CLEAN_START_HPP */
