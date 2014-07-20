#include "logging.hpp"
#include <log4cxx/logger.h>

using Wintermute::Logging;

W_DECLARE_SINGLETON(Logging);

log4cxx::LoggerPtr obtain_logger(const string& name)
{
  if (name == "root")
  {
    return log4cxx::Logger::getRootLogger();
  }
  return log4cxx::Logger::getLogger(name);
}

Logging::Logging()
{
}

Logging::~Logging()
{}

void Logging::setLevel(const Logging::Level& level)
{
  const int log4cxx_level = (uint) level * 1000;
  log4cxx::LoggerPtr logger = log4cxx::Logger::getRootLogger();
  logger->setLevel(log4cxx::Level::toLevel(log4cxx_level));
}

Logging::Level Logging::level() const
{
  log4cxx::LoggerPtr logger = log4cxx::Logger::getRootLogger();
  const int my_level = logger->getLevel()->toInt() / 1000;
  return (Logging::Level) my_level;
}

void Logging::error(const string& message, const string& name = "root")
{
  log4cxx::LoggerPtr logger = obtain_logger(name);
  logger->error(message);
}

void Logging::debug(const string& message, const string& name = "root")
{
  log4cxx::LoggerPtr logger = obtain_logger(name);
  logger->debug(message);
}

void Logging::info(const string& message, const string& name = "root")
{
  log4cxx::LoggerPtr logger = obtain_logger(name);
  logger->info(message);
}

void Logging::warn(const string& message, const string& name = "root")
{
  log4cxx::LoggerPtr logger = obtain_logger(name);
  logger->warn(message);
}

void Logging::trace(const string& message, const string& name = "root")
{
  log4cxx::LoggerPtr logger = obtain_logger(name);
  logger->trace(message);
}
