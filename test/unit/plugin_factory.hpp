#ifndef UNIT_TEST_PLUGIN_FACTORY_HPP
#define UNIT_TEST_PLUGIN_FACTORY_HPP

#include <QtCore/QObject>

class TestPluginFactory : public QObject
{
  Q_OBJECT;

private Q_SLOTS:
  void startApp();
};

#endif /* UNIT_TEST_PLUGIN_FACTORY_HPP */
