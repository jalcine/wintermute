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

#ifndef WINTERMUTE_ARGUMENTS_HPP
#define WINTERMUTE_ARGUMENTS_HPP

#include <Wintermute/Globals>
#include <QtCore/QVariantMap>
#include <QCommandLine>

namespace Wintermute
{
class ArgumentsPrivate;
class Application;

/**
 * @brief Auxiliary class to handle command-line arguments in a
 *        platform-agnostic way.
 *
 * @todo Finish documenting this class.
 *
 * @note This class uses some values from **QCommandLine**, a cross-platform
 *       argument processor.
 */
class Arguments : public QObject
{
	friend class ApplicationPrivate;

	Q_OBJECT
	Q_DECLARE_PRIVATE ( Arguments )

	QScopedPointer<ArgumentsPrivate> d_ptr;
	static Arguments* self;
	explicit Arguments();

public:
	virtual ~Arguments();

	/** @brief Provides a pointer to the active Arguments instance. */
	static Arguments* instance();

  /**
   * @brief Checks for the existence of an argument.
   * @param[in] argumentName The name of the argument to look for.
   * @retval boolean Whether or not the argument name exists.
   */
	bool hasArgument ( const QString& argumentName ) const;

  /**
   * @brief Obtains the argument's value.
   * @param[in] argumentName The name of the argument to obtain.
   * @retval QVariant The value of said argument.
   */
	QVariant argument ( const QString& argumentName ) const;

  /**
   * @brief Obtains all of the known command-line arguments.
   * @retval QVariantMap A map with all of the arguments from invocation.
   */
	QVariantMap arguments() const;

  /**
   * @brief Adds an boolean argument to the internal command line parser.
   * @param[in] optionChar The short letter name of the option.
   * @param[in] optionName The short letter name of the option.
   * @param[in] description A description about this option.
   * @param[in] flags A flag detonating the kind of flag this is.
   * @warning This doesn't throw an exception if the character or
   *          name collides.
   */
	void addOption ( const QChar& optionChar, const QString& optionName,
      const QString& description, QCommandLine::Flags flags );

  /**
   * @brief Adds an fixed-value argument to the internal command line parser.
   * @param[in] switchChar The short letter name of the switch.
   * @param[in] switchName The short letter name of the switch.
   * @param[in] description A description about this switch.
   * @param[in] flags A flag detonating the kind of flag this is.
   * @warning This doesn't throw an exception if the character or
   *          name collides.
   */
	void addSwitch ( const QChar& switchChar, const QString& switchName,
      const QString& description, QCommandLine::Flags flag );

  /**
   * @brief Adds an variable-value entry to the internal command line parser.
   * @param[in] parameterName The short letter name of the parameter.
   * @param[in] description A description about this switch.
   * @param[in] flag A flag detonating the kind of flag this is.
   */
	void addParameter ( const QString& parameterName, const QString& description,
      QCommandLine::Flags flag );

private:
	Q_SLOT void switchFound ( const QString& switchName );
	Q_SLOT void paramFound  ( const QString& paramName,  const QVariant& paramValue );
	Q_SLOT void optionFound ( const QString& optionName, const QVariant& optionValue );
	Q_SLOT void parseError  ( const QString& argumentName );
};
}

#define wntrArgs Wintermute::Arguments::instance()

#endif /* WINTERMUTE_ARGUMENTS_HPP */
