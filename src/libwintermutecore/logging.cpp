#include "logging.hpp"
#include <log4cxx/logger.h>
#include <log4cxx/basicconfigurator.h>
#include <log4cxx/patternlayout.h>
#include <log4cxx/fileappender.h>
#include <log4cxx/consoleappender.h>

using Wintermute::Logging;
using log4cxx::LoggerPtr;

W_DECLARE_SINGLETON(Logging);

LoggerPtr obtain_logger(const string& name)
{
  if (name == "root")
  {
    return log4cxx::Logger::getRootLogger();
  }
  return log4cxx::Logger::getLogger(name);
}

Logging::Logging()
{
  log4cxx::BasicConfigurator::configure();
  log4cxx::PatternLayout patternLayout;
  log4cxx::FileAppender fileAppender;
  log4cxx::ConsoleAppender consoleAppender;
  LoggerPtr rootLogger = obtain_logger("root");

  fileAppender.setFile("/var/log/wintermute.log");
  consoleAppender.setTarget(log4cxx::ConsoleAppender::getSystemOut());

  log4cxx::AppenderPtr fileAppenderPtr(&fileAppender);
  log4cxx::AppenderPtr consoleAppenderPtr(&consoleAppender);

  rootLogger->addAppender(fileAppenderPtr);
  rootLogger->addAppender(consoleAppenderPtr);

  info("Started logging service", "in.wintermute.logging");
}

Logging::~Logging()
{}

void Logging::setLevel(const Logging::Level& level)
{
  const int log4cxx_level = (uint) level * 1000;
  LoggerPtr logger = log4cxx::Logger::getRootLogger();
  logger->setLevel(log4cxx::Level::toLevel(log4cxx_level));
}

Logging::Level Logging::level() const
{
  LoggerPtr logger = log4cxx::Logger::getRootLogger();
  const int my_level = logger->getLevel()->toInt() / 1000;
  return (Logging::Level) my_level;
}

void Logging::error(const string& message, const string& name)
{
  LoggerPtr logger = obtain_logger(name);
  logger->error(message);
}

void Logging::debug(const string& message, const string& name)
{
  LoggerPtr logger = obtain_logger(name);
  logger->debug(message);
}

void Logging::info(const string& message, const string& name)
{
  LoggerPtr logger = obtain_logger(name);
  logger->info(message);
}

void Logging::warn(const string& message, const string& name)
{
  LoggerPtr logger = obtain_logger(name);
  logger->warn(message);
}

void Logging::trace(const string& message, const string& name)
{
  LoggerPtr logger = obtain_logger(name);
  logger->trace(message);
}
