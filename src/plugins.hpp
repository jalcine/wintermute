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

#include <QHash>
#include <QList>
#include <QSettings>
#include <QStringList>
#include <QPluginLoader>
#include <QProcess>
#include <QtDBus/QDBusAbstractAdaptor>
#include <QtDBus/QDBusMessage>
#include "adaptors.hpp"

namespace Wintermute {
    namespace Plugins {
        struct Factory;
        struct PluginBase;
        struct PluginInstance;

        /**
         * @brief Represents a named set of plugins.
         * @typedef PluginMap
         */
        typedef QHash<QString, PluginBase*> PluginList;

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
        class Factory : public QObject {
            Q_OBJECT
            Q_DISABLE_COPY(Factory)
            friend class PluginBase;

            signals:
                void pluginLoaded(const QString& ) const;
                void pluginUnloaded(const QString& ) const;
                void pluginCrashed(const QString& ) const;
                void initialized() const;
                void deinitialized() const;

            public slots:
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

            public:
                Factory();
                /**
                 * @brief Loads a plug-in.
                 * @fn loadPlugin
                 * @param
                 */
                static PluginBase* loadPlugin ( const QString& );
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
                static const QStringList loadedPlugins();

                /**
                 * @brief
                 *
                 * @fn allPlugins
                 * @return const QStringList
                 */
                static const QStringList allPlugins();

                /**
                 * @brief
                 *
                 * @fn instance
                 * @return const Factory *
                 */
                static Factory* instance();

            private:
                static PluginList s_plugins; /**< Holds a list  */
                static Factory* s_factory;
                QHash<const QString, PluginInstance*> m_plgnPool;

            private slots:
                /**
                 * @brief
                 *
                 * @fn loadStandardPlugin
                 */
                static void loadStandardPlugin();

                /**
                 * @brief
                 *
                 * @fn unloadStandardPlugin
                 */
                static void unloadStandardPlugin();
                /**
                 * @brief
                 *
                 * @fn doPluginLoad
                 * @param
                 */
                static void doPluginLoad(const QString&);
                /**
                 * @brief
                 *
                 * @fn doPluginUnload
                 * @param
                 */
                static void doPluginUnload(const QString&);
                /**
                 * @brief
                 *
                 * @fn doPluginCrash
                 * @param
                 */
                static void doPluginCrash(const QString&);
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
         * @class PluginBase plugins.hpp "plugins.hpp"
         * @
         */
        class PluginBase : public QObject {
            friend class Factory;
            friend class PluginInstanceAdaptor;
            Q_OBJECT
            Q_PROPERTY(const double Version READ version)
            Q_PROPERTY(const double CompatibleVersion READ compatVersion)
            Q_PROPERTY(const QString UUID READ uuid)
            Q_PROPERTY(const QString Name READ name)
            Q_PROPERTY(const QString Author READ author)
            Q_PROPERTY(const QString VendorName READ vendorName)
            Q_PROPERTY(const QString Description READ description)
            Q_PROPERTY(const QString WebPage READ webPage)
            Q_PROPERTY(const QStringList Dependencies READ dependencies)

            signals:
                /**
                 * @brief Raised when the plug-in is being initialized.
                 * This is usually raised right after the Core finishes initialization.
                 * @fn initializing
                 */
                void initializing() const;
                /**
                 * @brief Raised when the plug-in is being deinitialized.
                 * This is usually raised right before the Core begin to deinitialize.
                 * @fn deinitializing
                 */
                void deinitializing() const;

            private:
                QPluginLoader* m_plgnLdr; /**< Holds the plug-in loader object; it's hidden to inherited objects, but it's needed for the base object to operate. */
                QSettings* m_settings;

            public:
                /**
                 * @brief Empty, nullifying constructor.
                 * @fn PluginBase
                 */
                explicit PluginBase() : QObject(NULL), m_plgnLdr(NULL), m_settings(NULL) { }

                /**
                 * @brief Loads a plug-in based on the QPluginLoader.
                 * @fn PluginBase
                 * @param p_pl The plug-in to be loaded from disk.
                 */
                PluginBase(QPluginLoader* p_pl ) : QObject(p_pl), m_plgnLdr(p_pl), m_settings(NULL) { }

                /**
                 * @brief Default copy constructor.
                 * @fn PluginBase
                 * @param p_pb The plug-in to be copied.
                 */
                PluginBase(PluginBase const &p_pb) : QObject(p_pb.m_plgnLdr), m_plgnLdr(p_pb.m_plgnLdr), m_settings(p_pb.m_settings) {  }

                /**
                 * @brief Default deconstructor.
                 * @fn ~PluginBase
                 */
                virtual ~PluginBase();

                /**
                 * @brief Defines the version of the plug-in.
                 * @note Don't confuse with compatVersion(), this is merely the version of the plug-in.
                 * @see compatVersion ()
                 * @fn version
                 */
                const double version() const;

                /**
                 * @brief Defines the least required version of Wintermute needed for this plug-in to operate.
                 * @see version()
                 * @fn compatVersion
                 */
                const double compatVersion() const;

                /**
                 * @brief Represents a Universally Unique Identifier (UUID) for the plug-in.
                 * @note On Linux systems, the program 'uuidgen -t' could generate an unique UUID for you to define in your
                 *       plug-in specification file.
                 * @fn uuid
                 */
                const QString uuid() const;

                /**
                 * @brief Returns the canonical name of the plug-in. This is typically presented to the user.
                 * @fn name
                 * @see author()
                 * @see vendorName()
                 */
                const QString name() const;

                /**
                 * @brief Returns the name of the person, group, or organization that created this plug-in.
                 * @note It's permissible for you to return any of the following results:
                 * @code
                 * John Q. Doe
                 * Doe Developers <doe-devel@lists.doe.org>
                 * @endcode
                 * @note We encourage you to include an e-mail address in the 'Author' field of your plug-in specification file.
                 * @see vendorName()
                 * @fn author
                 */
                const QString author() const;

                /**
                 * @brief Returns the name of the person, group, or organization that packaged or distributed this plug-in.
                 * @note The syntax guideline for author() applies here as well.
                 * @fn vendorName
                 */
                const QString vendorName() const;

                /**
                 * @brief Returns a (preferably brief) description about the plug-in.
                 * @fn description
                 */
                const QString description() const;

                /**
                 * @brief Returns a URI to a web-page containing information about this plug-in.
                 * @fn webPage
                 */
                const QString webPage() const;

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
                 * @code
                 *       foo-bar (= 0.01)
                 *       foo-bar (>= 0.01) | hello-world (>> 0.02)
                 * @endcode
                 * @todo Use UUIDs as the canonical strings representing plug-ins. We'd need to have a Wiki page with a list of all currently known
                 * plug-ins as a central resource for developers. Add this page to the documentation once made.
                 * @fn dependencies
                 */
                const QStringList dependencies() const;

                /**
                 * @brief Determines if the dependencies of this plug-in have been satisified.
                 * @see dependencies
                 * @fn hasDependencies
                 */
                const bool hasDependencies() const;

                /**
                 * @brief Determines whether or not this plug-in is able to run without issues, in terms of versioning.
                 *
                 * @note This does <b>not</b> do a dependency check (not yet fully implemented).
                 *
                 * @fn isSupported
                 * @return true if this plug-in's compatiable version is less than or equal to this version of Wintermute.
                 *
                 * @todo More checking needs to be done to tell if it's compatible. (i.e: on major changes, make that a minimum compability value).
                 * @todo Implement checking of plug-in depenencies after checking versioning.
                 */
                const bool isSupported () const;
                /**
                 * @brief Returns a string that contains the absolute path to the plug-in.
                 * @fn path
                 * @deprecated This method is unnecessary towards the implementation of a plug-in.
                 */
                const QString path () const;

            protected slots:

                /**
                 * @brief Reimplement this method to define the initialization code of your plug-in.
                 * This is run after the initialized() signal is emitted.
                 * @fn initialize
                 */
                virtual void initialize() const = 0;

                /**
                 * @brief Reimplement this method to define the deinitialization code of your plug-in.
                 * This is run after the deinitialized() signal is emitted.
                 * @fn deinitialize
                 */
                virtual void deinitialize() const = 0;

            private slots:
                void doDeinitialize () const;
                void doInitialize() const;
        };

        class PluginInstance : public QObject {
            friend class Factory;
            Q_OBJECT
            Q_DISABLE_COPY(PluginInstance)
            Q_PROPERTY(const bool Active READ isActive)
            Q_PROPERTY(const QString Name READ name)

            public:
                PluginInstance();
                PluginInstance(const QString&, QSettings*);
                const bool isActive();
                const QString name();
                const QSettings* settings();

            public slots:
                void stop(const QDBusMessage = QDBusMessage());
                void start(const QDBusMessage = QDBusMessage());

            signals:
                void crashed(const QString&);
                void loaded(const QString&);
                void unloaded(const QString&);

            private:
                QProcess* m_prcss;
                const QString m_plgnName;
                QSettings* m_settings;
                void doCrashed(const QString&, const QDBusMessage = QDBusMessage());
                void doLoaded(const QString&, const QDBusMessage = QDBusMessage());
                void doUnloaded(const QString&, const QDBusMessage = QDBusMessage());

            private slots:
                void catchStart();
                void catchError(const QProcess::ProcessError& );
                void catchExit(int, const QProcess::ExitStatus& );
        };
    }
}

Q_DECLARE_INTERFACE(Wintermute::Plugins::PluginBase, "org.thesii.Wintermute.PluginBase");

#endif /* PLUGINS_HPP */
// kate: indent-mode cstyle; space-indent on; indent-width 4;
