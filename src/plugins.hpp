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
#include <QProcess>
#include "adaptors.hpp"

class QProcess;
class QSettings;
class QStringList;
class QDBusMessage;
class QPluginLoader;
class QDBusAbstractAdaptor;

namespace Wintermute {
namespace Plugins {
struct Factory;
struct Instance;
struct AbstractPlugin;
struct GenericPlugin;

/**
 * @brief Represents a named set of plugins.
 * @typedef PluginMap
 */
typedef QHash<QString, AbstractPlugin*> PluginList;

/**
 * @brief Abstract class representing the outlining information of a plug-in.
 *
 * The AbstractPlugin class is an abstract class laying out how the required bits of information
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
 * using Wintermute::Plugins::AbstractPlugin;
 *
 * namespace Foo {
 *      class Bar : public AbstractPlugin {
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
 * @class AbstractPlugin plugins.hpp "plugins.hpp"
 * @todo Add conflicting plug-ins as a specification addition.
 */
class AbstractPlugin : public QObject {
    Q_OBJECT
    Q_CLASSINFO("objectName","Plug-in")
    Q_PROPERTY(const double Version READ version)
    Q_PROPERTY(const double CompatibleVersion READ compatVersion)
    Q_PROPERTY(const QString UUID READ uuid)
    Q_PROPERTY(const QString Name READ name)
    Q_PROPERTY(const QString Author READ author)
    Q_PROPERTY(const QString VendorName READ vendorName)
    Q_PROPERTY(const QString Description READ description)
    Q_PROPERTY(const QString WebPage READ webPage)
    Q_PROPERTY(const QStringList Packages READ packages)
    Q_PROPERTY(const QStringList Plugins READ plugins)
    friend class Factory;
    friend class InstanceAdaptor;

signals:
    /**
     * @brief Raised when the plug-in is being initialized.
     * This is usually raised right after the Core finishes initialization.
     * @fn initializing
     */
    void started() const;
    /**
     * @brief Raised when the plug-in is being deinitialized.
     * This is usually raised right before the Core begin to deinitialize.
     * @fn deinitializing
     */
    void stopped() const;

private:
    mutable QPluginLoader* m_loader; /**< Holds the plug-in loader object; it's hidden to inherited objects, but it's needed for the base object to operate. */
    QSettings* m_settings;
    QSettings* m_configuration;

protected:
    void loadSettings(const QString&);
    const bool loadLibrary() const;
    const bool loadPlugins() const;
    const bool loadPackages() const;
    const bool loadRequiredComponents() const;

public:
    Q_DISABLE_COPY(AbstractPlugin)
    /**
     * @brief Empty, nullifying constructor.
     * @fn AbstractPlugin
     */
    explicit AbstractPlugin();

    /**
     * @brief Loads a plug-in based on the QPluginLoader.
     * @fn AbstractPlugin
     * @param p_pl The plug-in to be loaded from disk.
     */
    AbstractPlugin(QPluginLoader* );

    /**
     * @brief Default deconstructor.
     * @fn ~AbstractPlugin
     */
    virtual ~AbstractPlugin();

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
     * @brief Returns a QStringList of plug-ins that this plug-in requires to operate.
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
     * @note Use UUIDs as the canonical strings representing plug-ins. We'd need to have a Wiki page with a list of all currently known plug-ins as a central resource for developers. Add this page to the documentation once made.
     * @fn plugins
     */
    const QStringList plugins() const;

    /**
     * @brief Determines if the dependencies of this plug-in have been satisified.
     * @see plugins
     * @fn hasPlugins
     */
    const bool hasPlugins() const;

    /**
     * @brief Returns a QStringList of system packages that this plug-in requires to operate.
     * @fn packages
     */
    const QStringList packages() const;

    /**
     * @brief Determines whether or not the plug-in's libray has been loaded.
     * @fn isLoaded
     */
    const bool isLoaded() const;

    /**
     * @brief Determines if the packages of this plug-in have been satisified.
     * @see packages
     * @fn hasPackages
     */
    const bool hasPackages() const;

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
     * @brief Obtains an attribute from p_attrPath in the plug-in's configuration option set.
     * @param p_attrPath The name of the attribute.
     * @todo Allow a scoping of attributes (user-level, system-level).
     * @todo Allow pulling of attributes from other plug-ins.
     * @see setAttribute
     * @fn attribute
     */
    const QVariant attribute(const QString&) const;

    /**
     * @brief Changes an attribute at p_attrPath to p_attrVal to the plug-in's configuration option set.
     * @param p_attrPath The name of the attribute.
     * @param p_attrVal The new value of the attribute.
     * @see attribute
     * @todo Allow a scoping of attributes (user-level, system-level).
     * @todo Allow saving of attribute to other plug-ins.
     * @fn setAttribute
     */
    void setAttribute(const QString&, const QVariant&);

    /**
     * @brief Resets the attributes of the plug-in to default.
     * @fn resetAttributes
     */
    void resetAttributes();

protected slots:

    /**
     * @brief Reimplement this method to define the initialization code of your plug-in.
     * This is run after the initialized() signal is emitted.
     * @fn initialize
     */
    virtual void start() const = 0;

    /**
     * @brief Reimplement this method to define the deinitialization code of your plug-in.
     * This is run after the deinitialized() signal is emitted.
     * @fn deinitialize
     */
    virtual void stop() const = 0;

private slots:
    void doStart();
    void doStop();
};

/**
 * @brief Provides the Factory with a controllable instnace of a plugin.
 *
 * @see Factory, AbstractPlugin
 * @class Instance plugins.hpp "src/plugins.hpp"
 */
class Instance : public QObject {
    friend class Factory;
    Q_OBJECT
    Q_DISABLE_COPY(Instance)
    Q_CLASSINFO("objectName","Factory Plug-in Instance")
    Q_PROPERTY(const bool Active READ isActive)
    Q_PROPERTY(const QString Name READ name)

public:
    /**
     * @brief Null constructor.
     * @fn Instance
     */
    Instance();

    /**
     * @brief Default constructor.
     * @fn Instance
     * @param p_uuid The UUID of the plug-in.
     * @param p_stgs The QSettings of the plug-in.
     */
    explicit Instance(const QString&, QSettings*);

    /**
     * @brief Determines if the plug-in's currently active.
     * @fn isActive
     */
    const bool isActive();

    /**
     * @brief Obtains the UUID of the captured plug-in.
     * @fn uuid
     */
    const QString uuid();

    /**
     * @brief Obtains the friendly name of the captured plug-in.
     * @fn name
     */
    const QString name();

    /**
     * @brief Obtains the representative QSettings of the plug-in.
     * @fn settings
     */
    const QSettings* settings();

public slots:

    /**
     * @brief Halts the plug-in's process.
     * @fn stop
     */
    void stop();

    /**
     * @brief Starts the plug-in's process.
     * @fn start
     */
    void start();

signals:

    /**
     * @brief Emitted when the process of the plug-in experiences a crash.
     * @fn crashed
     */
    void crashed();
    void crashed(const QString&);

    /**
     * @brief Emitted when the process of the plug-in starts.
     * @fn started
     */
    void started();
    void started(const QString&);

    /**
     * @brief Emitted when the process of the plug-in stops.
     * @fn stopped
     */
    void stopped();
    void stopped(const QString&);

private:
    ~Instance();
    QProcess* m_process;
    const QString m_uuid;
    QSettings* m_settings;
    void doCrashed(const QString&);
    void doLoaded(const QString&);
    void doUnloaded(const QString&);

private slots:
    void catchStart();
    void catchError(const QProcess::ProcessError& );
    void catchExit(int, const QProcess::ExitStatus& );
};

/**
 * @brief Provides factory management of plug-ins.
 *
 * This static class manages anything and everything to do with plug-ins; from
 * loading, unloading, obtaining information and more about plugins. A lot of the
 * internal working dealing with plug-ins, however, are done within the AbstractPlugin
 * class itself. This merely manages the loaded plug-ins and executes prerequisties commands.
 *
 * @class Factory plugins.hpp "include/wintermute/plugins.hpp"
 * @see AbstractPlugin
 */
class Factory : public QObject {
    Q_OBJECT
    Q_DISABLE_COPY(Factory)
    Q_CLASSINFO("objectName","Plug-in Factory Singleton")
    friend class AbstractPlugin;

signals:
    /**
     * @brief Emitted when a plug-in has been successfully loaded into the system.
     * @fn pluginLoaded
     * @param p_uuid The UUID of the loaded plug-in.
     */
    void pluginLoaded(const QString& ) const;

    /**
     * @brief Emitted when a plug-in has been successfully unloaded from the system.
     * @fn pluginUnloaded
     * @param p_uuid The UUID of the unloaded plug-in.
     */
    void pluginUnloaded(const QString& ) const;

    /**
     * @brief Emitted when a plug-in experiences a sporadic crash.
     * @fn pluginCrashed
     * @param p_uuid The UUID of the faulty plug-in.
     */
    void pluginCrashed(const QString& ) const;

    /**
     * @brief Emitted when the factory's up and running.
     * @fn initialized
     */
    void started() const;

    /**
     * @brief Emitted when the factory's down for the count.
     * @fn deinitialized
     */
    void stopped() const;

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
    class ShellPlugin : public AbstractPlugin {
        friend class Factory;
        friend class AbstractPlugin;

    public:
        ShellPlugin();
        ShellPlugin(const QString& );
        ~ShellPlugin();

    private:
        virtual void start () const { }
        virtual void stop () const { }
    };

    Factory();
    /**
     * @brief Loads a plug-in.
     * @fn loadPlugin
     * @param
     */
    static AbstractPlugin* loadPlugin ( const QString&, const bool& = false );
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
     * @brief Obtains a list of all of the plug-ins that are installed and recognized by Wintermute.
     * @fn allPlugins
     * @return const QStringList
     */
    static const QStringList allPlugins();

    /**
     * @brief Obtains an instance of the Factory.
     * @fn instance
     * @return const Factory *
     */
    static Factory* instance();

    static AbstractPlugin* currentPlugin();

    static QVariant attribute(const QString& , const QString&);

    static void setAttribute(const QString&, const QString&, const QVariant& );

private:
    static PluginList s_plgnLst; /**< Holds a list  */
    static Factory* s_fctry;
    static AbstractPlugin* s_rtPlgn;
    QHash<const QString, Instance*> m_plgnPool;
    static QSettings* pluginSettings(const QString& );
    static const bool loadBackendPlugin(const QString& );

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
}
}

Q_DECLARE_INTERFACE(Wintermute::Plugins::AbstractPlugin, "org.thesii.Wintermute.AbstractPlugin");

#endif /* PLUGINS_HPP */
// kate: indent-mode cstyle; space-indent on; indent-width 4;
