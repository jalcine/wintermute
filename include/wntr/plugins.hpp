/**
 * @author Wintermute Developers <wintermute-devel@lists.launchpad.net>
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
 *
 */

#ifndef PLUGINS_HPP
#define PLUGINS_HPP

#include <QMap>
#include <QFile>
#include <QVector>
#include <QVariant>
#include <QSettings>
#include <QPluginLoader>
#include <wntr/config.hpp>

using namespace std;
using std::vector;
using std::map;

namespace Wintermute {
    namespace Plugins {
        struct Factory;
        struct PluginBase;

        /**
         * @brief Represents a named set of plugins.
         * @typedef PluginMap
         */
        typedef QMap<QString, PluginBase*> PluginList;

        /**
         * @brief Provides factory management of plug-ins.
         *
         * This abstract class manages anything and everything to do with plug-ins; from
         * loading, unloading, obtaining information and more about plugins. A lot of the
         * internal working dealing with plug-ins, however, are done within the PluginBase
         * class itself. This merely manages the loaded plug-ins and executes prerequisties commands.
         *
         * @class Factory plugins.hpp "include/wintermute/plugins.hpp"
         * @see PluginBase
         */
        class Factory {
            friend class PluginBase;
            public:
                /**
                 * @brief Starts the plug-in system.
                 * @fn Startup
                 */
                static void Startup();
                /**
                 * @brief Stops the plug-in system.
                 * @fn Shutdown
                 */
                static void Shutdown();
                /**
                 * @brief Loads a plug-in.
                 * @fn loadPlugin
                 * @param
                 */
                static const PluginBase* loadPlugin ( const string& );
                /**
                 * @brief Loads a plug-in into the system.
                 * @fn loadPlugin
                 * @overload
                 * @param
                 */
                static const PluginBase* loadPlugin ( const QFile* );
                /**
                 * @brief Unloads a plug-in from the system.
                 * @fn unloadPlugin
                 * @param
                 */
                static void unloadPlugin(const QString& );
                /**
                 * @brief Returns a list of all currently plug-ins with meta-data information.
                 * @fn loadedPlugins
                 * @returns A QList of plug-ins that are currently loaded into the system.
                 */
                PluginList const & loadedPlugins() { return s_allPlgns; }

            private:
                static PluginList s_allPlgns; /**< Holds pointers to all of the loaded plugins. */
        };

        /**
         * @brief Abstract class representing the outlining information of a plug-in.
         *
         * PluginBase is an abstract class laying out how the required bits of information
         * that a shared library or any other exectuable code that wish to add more functionality
         * to Wintermute. It's important that plug-in developers provide implementations of the
         * virtually defined methods, or their plug-in will fail to load. It is required that
         * in the definition of your plug-in's code (not within the header files, but rather in the source code)
         * that you include the header file &lt;QtPlugin&gt; and define the Q_EXPORT_PLUGIN2() macro. Your
         * plug-in must also define a copy constructor.
         *
         * @code
         * #include <QObject>
         * #include <QtPlugin>
         * #include <wntr/plugins.hpp>
         *
         * using namespace Wintermute::Plugins;
         * using Wintermute::Plugins::PluginBase;
         *
         * namespace Foo {
         *      class Bar : public PluginBase {
         *          public:
         *              Bar() : m_magicNumber(0) { }
         *              Bar(const Bar &p_bar) : m_magicNumber(p_bar.magicNumber) { }
         *              virtual ~Bar();
         *
         *              /// definition of virtual methods
         *
         *          private:
         *              int m_magicNumber;
         *      };
         * }
         *
         * Q_EXPORT_PLUGIN2(Bar, Foo::Bar)
         * @endcode
         *
         * @attention It's important to note that you must define the Q_EXPORT_PLUGIN2() macro <i>outside</i>
         * the scope of any namespace declarations.
         *
         * @class PluginBase plugins.hpp "include/wntr/plugins.hpp"
         * @
         */
        class PluginBase : public QObject {
            friend class Factory;
            Q_OBJECT
            Q_PROPERTY(const double Version READ version)
            Q_PROPERTY(const double CompatibleVersion READ compatVersion)
            Q_PROPERTY(const QString UUID READ uuid)
            Q_PROPERTY(const QString Name READ name)
            Q_PROPERTY(const QString Author READ author)
            Q_PROPERTY(const QString VendorName READ vendorName)
            Q_PROPERTY(const QString Description READ description)
            Q_PROPERTY(const QString WebPage READ webPage)
            Q_PROPERTY(const QString Dependencies READ dependencies)

            signals:
                /**
                 * @brief Raised when the plug-in is being initialized.
                 * This is usually raised right after the Core finishes initialization.
                 * @fn initializing
                 */
                void initializing();
                /**
                 * @brief Raised when the plug-in is being deinitialized.
                 * This is usually raised right before the Core begin to deinitialize.
                 * @fn deinitializing
                 */
                void deinitializing();

            private:
                QPluginLoader* m_plgnLdr; /**< Holds the plug-in loader object; it's hidden to inherited objects, but it's needed for the base object to operate. */

            public:
                /**
                 * @brief Empty, nullifying constructor.
                 * @fn PluginBase
                 */
                explicit PluginBase() : QObject(NULL), m_plgnLdr(NULL) { }

                /**
                 * @brief Loads a plug-in based on the QPluginLoader.
                 * @fn PluginBase
                 * @param p_pl The plug-in to be loaded from disk.
                 */
                PluginBase(QPluginLoader* p_pl ) : QObject(p_pl), m_plgnLdr(p_pl) { }

                /**
                 * @brief Default copy constructor.
                 * @fn PluginBase
                 * @param p_pb The plug-in to be copied.
                 */
                PluginBase(PluginBase const &p_pb) : QObject(p_pb.m_plgnLdr), m_plgnLdr(p_pb.m_plgnLdr){  }

                /**
                 * @brief Default deconstructor.
                 * @fn ~PluginBase
                 */
                virtual ~PluginBase() { delete m_plgnLdr; };

                /**
                 * @brief Defines the version of the plug-in.
                 * @note Don't confuse with compatVersion(), this is merely the version of the plug-in.
                 * @see compatVersion ()
                 * @fn version
                 */
                virtual const double version() const = 0;

                /**
                 * @brief Defines the least required version of Wintermute needed for this plug-in to operate.
                 * @see version()
                 * @fn compatVersion
                 */
                virtual const double compatVersion() const = 0;

                /**
                 * @brief Represents a Universally Unique Identifier (UUID) for the plug-in.
                 * @note On Linux systems, the program 'uuidgen -t' could generate an unique UUID for you to use.
                 *       It's recommend that you fill this value, otherwise a run-time warning would be invoked.
                 * @fn uuid
                 */
                virtual const QString uuid() const = 0;

                /**
                 * @brief Returns the canonical name of the plug-in. This is typically presented to the user.
                 * @fn name
                 * @see author()
                 * @see vendorName()
                 */
                virtual const QString name() const = 0;

                /**
                 * @brief Returns the name of the person, group, or organization that created this plug-in.
                 * @note It's permissible for you to return any of the following results:
                 * @code
                 * John Q. Doe
                 * Doe Developers <doe-devel@lists.doe.org>
                 * @endcode
                 * @note We encourage you to include an e-mail address in the 'Author' field.
                 * @see vendorName()
                 * @fn author
                 */
                virtual const QString author() const = 0;

                /**
                 * @brief Returns the name of the person, group, or organization that packaged or distributed this plug-in.
                 * @note The syntax guideline for author() applies here as well.
                 * @fn vendorName
                 */
                virtual const QString vendorName() const = 0;

                /**
                 * @brief Returns a (preferably brief) description about the plug-in.
                 * @fn description
                 */
                virtual const QString description() const = 0;

                /**
                 * @brief Returns a URI to a web-page containing information about this plug-in.
                 * @fn webPage
                 */
                virtual const QString webPage() const = 0;

                /**
                 * @brief Returns a comma-separated list of plug-ins that this plug-in requires to operate.
                 *
                 * @note If the plug-ins that this plug-in depends on are installed on the system and allowed to be
                 * activated, then they'll be loaded before this plug-in to satisfy this dependency requirement.
                 *
                 * @note The comma is required if you are listing more than one plug-in.
                 *
                 * @note Version information is not yet supported.
                 * @todo Implement depenency checking with strings like (something similar to dpkg's versioning scheme or pkg-config's):
                        foo-bar (= 0.01)
                        foo-bar (>= 0.01) | hello-world (>> 0.02)
                 * @todo Use UUIDs as the canonical strings representing plug-ins. We'd need to have a Wiki page with a list of all currently known
                 * plug-ins as a central resource for developers. Add this page to the documentation once made.
                 * @fn dependencies
                 */
                virtual const QString dependencies() const = 0;

                /**
                 * @brief Determines whether or not this plug-in is able to run without issues, in terms of versioning.
                 * @note This does <b>not</b> do a dependency check (not yet fully implemented).
                 * @fn isSupported
                 * @return true if this plug-in's compatiable version is less than or equal to this version of Wintermute.
                 * @todo More checking needs to be done to tell if it's compatible. (i.e: on major changes, make that a minimum compability value).
                 * @todo Implement checking of plug-in depenencies after checking versioning.
                 */
                const bool isSupported () const { return WINTERMUTE_VERSION >= compatVersion (); }

                /**
                 * @brief Returns a string that contains the absolute path to the plug-in.
                 * @fn path
                 * @deprecated This method is unnecessary towards the implementation of a plug-in.
                 */
                const QString path () const { return m_plgnLdr->fileName (); }

            protected slots:

                /**
                 * @brief Reimplement this method to define the initialization code of your plug-in.
                 * This is run after the initialized() signal is emitted.
                 * @fn initialize
                 */
                virtual void initialize() = 0;

                /**
                 * @brief Reimplement this method to define the deinitialization code of your plug-in.
                 * This is run before the deinitialized() signal is emitted.
                 * @fn deinitialize
                 */
                virtual void deinitialize() = 0;

            private slots:
                void doDeinitialize () {
                    deinitialize ();
                    emit deinitializing ();
                    Factory::unloadPlugin(uuid());
                }

                void doInitializing() {
                    initialize ();
                    emit initializing ();
                }
        };

    }
}

Q_DECLARE_INTERFACE(Wintermute::Plugins::PluginBase, "org.thesii.Wintermute.PluginBase");

#endif /* PLUGINS_HPP */
// kate: indent-mode cstyle; space-indent on; indent-width 4;
