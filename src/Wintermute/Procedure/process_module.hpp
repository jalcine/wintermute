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

#ifndef WINTERMUTE_PROCEDURE_PROCESS_MODULE_HPP
#define WINTERMUTE_PROCEDURE_PROCESS_MODULE_HPP

#include <Wintermute/Application>
#include <Wintermute/Procedure/Module>
#include "module.hpp"

namespace Wintermute
{
namespace Procedure
{
/**
 * @brief A Module project representing this process.
 * @sa Wintermute::Procedure::Module
 *
 * ProcessModule serves as a means of exposing this current process over
 * the network. Typically the first module created by a running process,
 * it connects it to the other processes quickly.
 */
class ProcessModule : public Module
{
    Q_OBJECT
    Q_DISABLE_COPY ( ProcessModule )
    Q_INTERFACES (Wintermute::Procedure::Module)

    friend class Wintermute::ApplicationPrivate;
    explicit ProcessModule();

  public:
    virtual ~ProcessModule(); ///< @brief Destructor.
    ///< @brief Starts this process module.
    Q_SLOT virtual void start();
    ///< @brief Stops this process module.
    Q_SLOT virtual void stop();

  protected:
    ///< @brief Begins the act of quitting and starting this process over.
    void reboot();
    ///< @brief Begins the act of the quitting this process altogether.
    void quit ( const int exitcode = 0 );
};
}
}

#endif
