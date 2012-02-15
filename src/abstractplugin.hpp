/**
 * @file abstractplugin.hpp
 * @author Wintermute Developers <wintermute-devel@lists.launchpad.net>
 *
 * @legalese
 * This program is free software; you can redistribute it and/or
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

#ifndef _ABSTRACTPLUGIN_HPP_
#define _ABSTRACTPLUGIN_HPP_

// Local
#include "adaptors.hpp"

// Qt
#include <QHash>
#include <QSettings>
#include <QPluginLoader>

namespace Wintermute {
namespace Plugins {

// Forward declarations
class Factory;

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
class AbstractPlugin : public QObject
{
    friend class Factory;
    friend class PluginHandleAdaptor;

    Q_OBJECT
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

    private:
        mutable QPluginLoader *m_plgnLdr; /**< Holds the plug-in loader object; it's hidden to inherited objects, but it's needed for the base object to operate. */
        QSettings *m_sttngs;
        QSettings *m_cnfg;

    private slots:
        bool loadPlugins() const;
        bool loadPackages() const;
	void loadSettings(const QString& );
        void doStart();
        void doStop();

    protected:
        const bool loadLibrary() const;
        const bool loadRequiredComponents() const;

    public:
        /**
         * @brief Empty, nullifying constructor.
         */
        explicit AbstractPlugin() : QObject(NULL), m_plgnLdr(NULL), m_sttngs(NULL), m_cnfg(NULL) {}

        /**
         * @brief Loads a plug-in based on the QPluginLoader.
         * @param p_pl The plug-in to be loaded from disk.
         */
        AbstractPlugin(QPluginLoader *p_pl);

        /**
         * @brief Default copy constructor.
         * @param p_pb The plug-in to be copied.
         */
        AbstractPlugin(AbstractPlugin const &p_pb);

        /**
         * @brief Default deconstructor.
         */
        virtual ~AbstractPlugin();

        /**
         * @brief Defines the version of the plug-in.
         * @note Don't confuse with compatVersion(), this is merely the version of the plug-in.
         * @see compatVersion ()
         */
        double version() const;

        /**
         * @brief Defines the least required version of Wintermute needed for this plug-in to operate.
         * @see version()
         */
        double compatVersion() const;

        /**
         * @brief Represents a Universally Unique Identifier (UUID) for the plug-in.
         * @note On Linux systems, the program 'uuidgen -t' could generate an unique UUID for you to define in your
         *       plug-in specification file.
         */
        QString uuid() const;

        /**
         * @brief Returns the canonical name of the plug-in. This is typically presented to the user.
         * @see author()
         * @see vendorName()
         */
        QString name() const;

        /**
         * @brief Returns the name of the person, group, or organization that created this plug-in.
         * @note It's permissible for you to return any of the following results:
         * @code
         * John Q. Doe
         * Doe Developers <doe-devel@lists.doe.org>
         * @endcode
         * @note We encourage you to include an e-mail address in the 'Author' field of your plug-in specification file.
         * @see vendorName()
         */
        QString author() const;

        /**
         * @brief Returns the name of the person, group, or organization that packaged or distributed this plug-in.
         * @note The syntax guideline for author() applies here as well.
         */
        QString vendorName() const;

        /**
         * @brief Returns a (preferably brief) description about the plug-in.
         */
        QString description() const;

        /**
         * @brief Returns a URI to a web-page containing information about this plug-in.
         */
        QString webPage() const;

        /**
         * @brief Returns a QStringList of plug-ins that this plug-in requires to operate.
         *
         * @note If the plug-ins that this plug-in depends on are installed on the system and allowed to be
         * activated, then they'll be loaded before this plug-in to satisfy this dependency requirement.
         *
         * @note The comma is required if you are listing more than one plug-in.
         *
         * @note Version information is not yet supported.
         * @todo Implement depenency checking with triples like (something similar to dpkg's versioning scheme or pkg-config's):
         * @code
         *       foo-bar (= 0.01)
         *       foo-bar (>= 0.01) | hello-world (>> 0.02)
         * @endcode
         * @note Use UUIDs as the canonical strings representing plug-ins. We'd need to have a Wiki page with a list of all currently known plug-ins as a central resource for developers. Add this page to the documentation once made.
         */
        QStringList plugins() const;

        /**
         * @brief Determines if the dependencies of this plug-in have been satisified.
         * @see plugins
         */
        bool hasPlugins() const;

        /**
         * @brief Returns a QStringList of system packages that this plug-in requires to operate.
         */
        QStringList packages() const;

        /**
         * @brief Determines if the packages of this plug-in have been satisified.
         * @see packages
         */
        bool hasPackages() const;

        /**
         * @brief Determines whether or not this plug-in is able to run without issues, in terms of versioning.
         *
         * @note This does <b>not</b> do a dependency check (not yet fully implemented).
         *
         * @return true if this plug-in's compatiable version is less than or equal to this version of Wintermute.
         *
         * @todo More checking needs to be done to tell if it's compatible. (i.e: on major changes, make that a minimum compability value).
         * @todo Implement checking of plug-in depenencies after checking versioning.
         */
        bool isSupported() const;

        /**
         * @brief Obtains an attribute from p_attrPath in the plug-in's configuration option set.
         * @param p_attrPath The name of the attribute.
         * @todo Allow a scoping of attributes (user-level, system-level).
         * @todo Allow pulling of attributes from other plug-ins.
         * @see setAttribute
         */
        QVariant attribute(const QString &p_attrPath) const;

        /**
         * @brief Changes an attribute at p_attrPath to p_attrVal to the plug-in's configuration option set.
         * @param p_attrPath The name of the attribute.
         * @param p_attrVal The new value of the attribute.
         * @see attribute
         * @todo Allow a scoping of attributes (user-level, system-level).
         * @todo Allow saving of attribute to other plug-ins.
         */
        void setAttribute(const QString &p_attrPath, const QVariant &p_attrVal);

        /**
         * @brief Resets the attributes of the plug-in to default.
         */
        void resetAttributes();

    signals:
        /**
        * @brief Raised when the plug-in is being initialized.
        * This is usually raised right after the Core finishes initialization.
        */
        void started() const;

        /**
        * @brief Raised when the plug-in is being deinitialized.
        * This is usually raised right before the Core begin to deinitialize.
        */
        void stopped() const;

    protected slots:
        /**
         * @brief Reimplement this method to define the initialization code of your plug-in.
         * This is run after the initialized() signal is emitted.
         */
        virtual void start() const = 0;

        /**
         * @brief Reimplement this method to define the deinitialization code of your plug-in.
         * This is run after the deinitialized() signal is emitted.
         */
        virtual void stop() const = 0;
};

/**
 * @brief Represents a named set of plugins.
 */
typedef QHash<QString, AbstractPlugin*> PluginTable;

} // namespaces
}
Q_DECLARE_INTERFACE(Wintermute::Plugins::AbstractPlugin, "org.thesii.Wintermute.AbstractPlugin")

#endif // _ABSTRACTPLUGIN_HPP_
