#ifndef TEST_COMMAND_LINE_HPP
#define TEST_COMMAND_LINE_HPP

#include <QObject>

class QProcess;

class CommandLineUnitTest : public QObject {
  Q_OBJECT;

  private:
    QProcess* process;

  private slots:
    void cleanup();
    void showHelp();
    void showVersion();
    void showInvalidArgument();
};

#endif /* TEST_COMMAND_LINE_HPP */
