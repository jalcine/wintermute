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

#ifndef CORE_HPP
#define CORE_HPP

#include <QApplication>
#include <QVariantMap>

namespace Wintermute {
    struct Core;

    /**
     * @brief
     *
     * @class Core wintermute.hpp "include/wintermute/wintermute.hpp"
     */
    class Core : public QObject {
        Q_OBJECT
        public:
            static void Configure ( int& , char ** );
            /**
             * @brief Initializes the system.
             * Does the first bit of initialization work for the core process of Wintermute.
             * @todo Consider testing whether or not Wintermute's running as a daemon and invoke a sub process.
             * @fn Initialize
             */
            static void Initialize();
            /**
             * @brief Deinitializes the system.
             * Cleans up all of the work for the core processes and runs the approriate disconnection methods.
             * @fn Deinitialize
             */
            static void Deinitialize ();
            /**
             * @brief
             * @fn appInstance
             */
            static QApplication* appInstance();

        protected:
            static QApplication* s_app;
            static QVariantMap* s_args;

        private:
            static void configureCommandLine();
    };
}

#endif /* CORE_HPP */
