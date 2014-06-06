/*
 * @author Jacky Alciné <me@jalcine.me>
 * @copyright © 2011, 2012, 2013, 2014 Jacky Alciné <me@jalcine.me>
 * @if 0
 * Wintermute is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * Wintermute is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Wintermute.  If not, see <http://www.gnu.org/licenses/>.
 * @endif
 **/

#include <WintermuteTestDriver>
#include <Wintermute/Application>
#include <Wintermute/Arguments>

class TestArguments : public Wintermute::Arguments
{
  Q_OBJECT;
public:
  TestArguments() : Arguments()
  {
  }

  virtual ~TestArguments()
  {
  }
};

/**
 * The purpose of this test is to ensure that Wintermute's ability to parse
 * command-line arguments is robust and up to par with what we'd expect from
 * it.
 */
class ArgumentsUnitTest : public QObject
{
  Q_OBJECT;
  TestArguments* argParser;

  Q_SLOT void init()
  {
    argParser = nullptr;
  }

  Q_SLOT void cleanup()
  {
    if (argParser || argParser != nullptr || argParser != 0)
    {
      delete argParser;
      argParser = nullptr;
    }
  }

  Q_SLOT void provideFlagOptions()
  {
#ifndef ARGUMENTS_REFACTOR
    argParser->addFlag("foobar", "f", "This is a foobaring flag.");
    argParser->provide(QStringList() << "--foobar");
    QVERIFY(argParser->hasArgument("foobar"));
    QVariant theArgument = argParser->argument("foobar");
    QVERIFY(theArgument.toBool() == true);
#else
    SKIP_NOT_IMPLEMENTED
#endif
  }

  Q_SLOT void provideSingleOptions()
  {
#ifndef ARGUMENTS_REFACTOR
    argParser->addOption("foobar", "f", "This is a foobaring option.");
    argParser->provide(QStringList() << "--foobar=karate");
    QVERIFY(argParser->hasArgument("foobar"));
    QVariant theArgument = argParser->argument("foobar");
    QVERIFY(theArgument.toString() == QString("karate"));
#else
    SKIP_NOT_IMPLEMENTED
#endif
  }

  Q_SLOT void provideMultiOptions()
  {
#ifndef ARGUMENTS_REFACTOR
    QStringList values = QStringList() << "cookie", "jazz", "karate";
    argParser->addOptionList("foobar", "f", "This is a foobaring option list.");
    argParser->provide(QStringList() << QString("--foobar=") + values.join(","));
    QVERIFY(argParser->hasArgument("foobar"));
    QStringList theArgument = argParser->argument("foobar");
    QVERIFY(theArgument == values); 
#else
    SKIP_NOT_IMPLEMENTED
#endif
  }

  Q_SLOT void reparseArguments()
  {
#ifndef ARGUMENTS_REFACTOR
#else
    SKIP_NOT_IMPLEMENTED
#endif
  }
};

QTEST_APPLESS_MAIN ( ArgumentsUnitTest )
#include "arguments.moc"
