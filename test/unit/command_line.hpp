#ifndef TEST_COMMAND_LINE_HPP
#define TEST_COMMAND_LINE_HPP

#include <QObject>

class TestCommandLine : public QObject {
  Q_OBJECT;

  private slots:
    void showHelp();
    void showVersion();
    void showInvalidArgument();
};

#endif /* TEST_COMMAND_LINE_HPP */
