/**
 * @file linguistics.hpp
 * @author: Wintermute Development <wntr-devel@thesii.org>
 *
 * @section lcns Licensing
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
 *
 * @date March 29, 2011, 3:08 PM
 */

#ifndef WNTRDATA_LINGUISTICS_HPP
#define WNTRDATA_LINGUISTICS_HPP

// Qt includes
#include <QString>
#include <QObject>

// local includes
#include "config.hpp"
#include "lexical/lexical.hpp"
#include "syntax/syntax.hpp"

namespace Wintermute
{
namespace Data
{
namespace Linguistics
{

/**
 * @brief An abstract static class representing the current configuration of the linguistics data.
 * All configuration variables and their access methods specific for linguistic part of WntrData
 * are defined here.
 */
class System
{
private:
    static QString s_dir; /**< Holds the location of the linguistic info. */
    static QString s_lcl; /**< Holds the current locale in use. */

public:
    /**
     * @brief Changes the locale.
     * Sets the default locale to be used as the standard locale.
     * @param locale The locale to set.
     */
    static void setLocale (const QString);

    /**
     * @brief Changes root directory.
     * Sets the root directory to find data for linguistics from.
     * @param root The root directory.
     */
    static void setDirectory (const QString);

    /**
     * @brief Gets current locale used.
     * Returns the locale considered to be the standard of the linguistics system.
     * @fn getLocale
     * @return string String with the current locale.
     */
    inline static const QString locale() {
        return s_lcl;
    }

    /**
     * @brief Obtains a list of all of the locales known to the system.
     * @fn locales
     */
    static const QStringList locales();

    /**
     * @brief Gets linguistics directory.
     * Returns the directory that's considered to be the absolute directory for loading linguistics information.
     * @fn getDirectory
     * @return string
     */
    inline static const QString directory() {
        return s_dir;
    }

    /**
     * @brief Loads the system with a specific storage directory and locale.
     * @fn load
     */
    static void load (const QString = s_dir, const QString = s_lcl);

    /**
     * @brief Unloads all information from the system.
     * @fn unload
     */
    static void unload();
};

}
}
}
#endif  /* LINGUISTICS_HPP */
// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
