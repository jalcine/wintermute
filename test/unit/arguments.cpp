/**
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

  Q_SLOT void init()
  {
  }

  Q_SLOT void provideFlagOptions()
  {
    QSKIP("Not defined yet.", SkipSingle);
  }

  Q_SLOT void provideSwitchOptions()
  {
    QSKIP("Not defined yet.", SkipSingle);
  }

  Q_SLOT void provideSingleOptions()
  {
    QSKIP("Not defined yet.", SkipSingle);
  }

  Q_SLOT void provideMultiOptions()
  {
    QSKIP("Not defined yet.", SkipSingle);
  }

  Q_SLOT void reparseArguments()
  {
    QSKIP("Not defined yet.",SkipSingle);
  }

  Q_SLOT void cleanup()
  {
  }
};

QTEST_APPLESS_MAIN ( ArgumentsUnitTest )
#include "arguments.moc"
