/**
 * @file model.hpp
 * @author Jacky Alcine <jacky.alcine@thesii.org>
 * @date Sun, 30 Oct 2011 21:54:16
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
 */

#ifndef WNTRDATA_SYSTEM_HPP
#define WNTRDATA_SYSTEM_HPP

// Qt includes
#include <QObject>

namespace Wintermute {
namespace Data {

class System : public QObject {
    friend class SystemAdaptor;
    friend class SystemInterface;
    Q_OBJECT
    Q_DISABLE_COPY ( System )

private:
    static System* s_inst;
    QString m_dir;
    System();

public:
    /**
     * @brief Obtains the directory that of which the data files are stored.
     * @fn getDirectory
     * @return const QString
     */
    static const QString directory();

    /**
     * @brief Changes the working directory.
     * @fn setDirectory
     * @param const QString
     */
    static void setDirectory ( const QString& );

    /**
     * @brief Obtains an instance of the data system object.
     * @fn instance
     * @return const Configuration
     */
    static System* instance();

signals:
    /**
     * @brief
     *
     * @fn started
     */
    void started();

    /**
     * @brief
     *
     * @fn stopped
     */
    void stopped();

public slots:
    /**
     * @brief Initializes the data services.
     * @fn Initialize
     */
    static void stop();

    /**
     * @brief Deinitializes the data services.
     * @fn Deinitialize
     */
    static void start();

    static void registerDataTypes();
};

}
}

#endif
// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
