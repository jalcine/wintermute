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

#include <Wintermute/Version>
#include <WintermuteTestDriver>

using Wintermute::Version;

class VersionUnitTest : public QObject
{
  Q_OBJECT;
private:
  Q_SLOT void formVersionFromString()
  {
    QVERIFY(Version::fromString("0.1.0").major == 0);
    QVERIFY(Version::fromString("0.1.0").minor == 1);
    QVERIFY(Version::fromString("0.1.0").patch == 0);
  }

  Q_SLOT void formStringFromVersion()
  {
    QVERIFY(Version::fromString("0.1.0").toString() == "0.1.0");
  }

  Q_SLOT void compareGreaterVersion()
  {
    Version versionA = Version::fromString("3.2.1");
    Version versionB = Version::fromString("1.2.3");
    QVERIFY(versionA > versionB);
  }

  Q_SLOT void compareEqualVersion()
  {
    Version versionA = Version::fromString("3.2.1");
    Version versionB = Version::fromString("3.2.1");
    QVERIFY(versionA == versionB);
  }

  Q_SLOT void compareLesserVersion()
  {
    Version versionA = Version::fromString("1.2.3");
    Version versionB = Version::fromString("3.2.1");
    QVERIFY(versionA < versionB);
  }
};

QTEST_APPLESS_MAIN ( VersionUnitTest );
#include "version.moc"
