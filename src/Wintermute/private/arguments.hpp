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

#include <QtCore/QObject>
#include <QCommandLine>

namespace Wintermute
{
class ArgumentsPrivate
{
public:
  QCommandLine* args;
  QVariantMap arguments;

  ArgumentsPrivate ( QObject* parent ) :
    args ( new QCommandLine ( parent ) )
  {
    args->enableVersion ( true );
    args->enableHelp ( true );
    addArguments();
  };

  /**
   * @fn addArguments
   *
   * This private method handles the work of calling the necessary
   * methods to grab all of the valid arguments from the command line.
   *
   * TODO: Allow dynamic appending to list of default methods.
   */
  void addArguments()
  {
    args->addOption ( 'm', "mode",
                      "Defines the mode that of which Wintermute will operate as.",
                      QCommandLine::Mandatory );
    args->addOption ( 'f', "fork", "Runs this process in the background.",
                      QCommandLine::Optional );
    args->addOption ( 'p', "plugin", "Defines the plugin UUID to be used.",
                      QCommandLine::Optional );
  }
};
}

