/**
 * @author Jacky Alcine <jackyalcine@gmail.com>
 *
 * @legalese
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 * @endlegalese
 */
#ifndef WINTERMUTE_HPP
#define WINTERMUTE_HPP

#include <QCoreApplication>
#include <iostream>
#include <csignal>
#include <wntrntwk.hpp>
#include <wntrdata.hpp>
#include <wntrling.hpp>
#include <boost/program_options.hpp>
#include "ipc.hpp"
#include "plugins.hpp"

using namespace std;
using namespace Wintermute;
using namespace Wintermute::Linguistics;
using namespace boost::program_options;

using std::cout;
using std::endl;
using boost::program_options::variables_map;
using boost::program_options::options_description;

namespace Wintermute {
    struct Core;

    /**
     * @brief
     *
     * @class Core wintermute.hpp "include/wintermute/wintermute.hpp"
     */
    class Core {
    public:
        static void Configure(int& , char **);
        /**
         * @brief
         *
         * @fn Initialize
         */
        static void Initialize();
        /**
         * @brief
         *
         * @fn Deinitialize
         */
        static void Deinitialize ();
        /**
         * @brief Manages command line.
         * Mananges the command toggles and what-not of Wintermute.
         * @fn manageCmdLine
         * @todo Allow an appending of command-line options on-the-fly; may need to create an abstract class for it.
         */
        static void manageCmdLine(variables_map& , options_description&);

    protected:
        static QCoreApplication* m_app;
    };
}

#endif /* WINTERMUTE_HPP */
