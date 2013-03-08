#ifndef UNIT_TEST_COMMAND_LINE_HPP
#define UNIT_TEST_COMMAND_LINE_HPP

#include <QtCore/QObject>
#include <QtCore/QProcess>
#include <Wintermute/Testing>

class TestCommandLine : public QObject {
  Q_OBJECT;

  private:
    QProcess* buildProcess();
    QString applicationPath() const { return WINTERMUTE_EXECUTABLE; }

  private Q_SLOTS:
    void passInVersion();
    void passInHelp();
    void passInvalidArguments();
};

#endif /* UNIT_TEST_COMMAND_LINE_HPP */
