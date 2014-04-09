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

/**
 * @headerfile Wintermute/logging.hpp <Wintermute/Logging>
 * @brief      Definition of Wintermute::Logging class.
 */

#ifndef WINTERMUTE_LOGGING_HPP
#define WINTERMUTE_LOGGING_HPP

#include <QtCore/QtDebug>
#include <Wintermute/Globals>
#include <Logger>

namespace Wintermute
{
class LoggingPrivate;

/**
 * @brief Represents the underlying logger class used by Wintermute.
 */
typedef ::Log4Qt::Logger Logger;

/**
 * @brief Auxiliary class for logging in Wintermute.
 * 
 * This class provides the provided classes and tools necessary to handle
 * logging using the [log4qt][] library in conjunction with Qt's own logging
 * facilities.
 *
 * [log4qt]: http://gitorious.org/log4qt/log4qt.git
 */
class Logging : public QObject
{
	Q_OBJECT
	Q_DECLARE_PRIVATE ( Logging )

	friend class ApplicationPrivate;

	QScopedPointer<LoggingPrivate> d_ptr;
	static Logging* self;
	explicit Logging();
	static Logging* instance();

public:
	virtual ~Logging();

  /**
   * @brief Obtains a logger with a designated name.
   * @param[in] loggerName The name of the logger.
   * @retval Logger* The logger instance for use.
   */
	static Logger* obtainLogger ( const QString& loggerName );

  /**
   * @brief Obtains a logger for the designated `QObject`.
   * @param[in] object The object to use.
   * @retval Logger* The logger instance for use.
   */
	static Logger* obtainLogger ( const QObject* object = nullptr );

  /**
   * @brief Obtains the root logger.
   * @retval Logger* The root logger instance.
   */
	static Logger* obtainRootLogger();
};
}

/**
 * @brief Obtains a logger for the specific QObject.
 * @param[in] qobj An object based on QObject.
 * @retval Logger* A logging instance to use.
 */
#define wlog(qobj) Wintermute::Logging::obtainLogger(qobj)

/**
 * @brief Quickly grabs a logger and send text to debug output.
 * @param[in] qobj An object based on QObject.
 * @param[in] text The text to be displayed for debugging.
 *
 * Obtains a logger for the specific QObject and then prints out a
 * line of text for debubgging view.
 */
#define wdebug(qobj, text) wlog(qobj)->debug(text)

/**
 * @brief Quickly grabs a logger and send text to warning output.
 * @param qobj An object based on QObject.
 * @param text The text to be displayed as a warning.
 */
#define wwarn(qobj, text) wlog(qobj)->warn(text)

/**
 * @brief Quickly grabs a logger and send text to informational output.
 * @param qobj An object based on QObject.
 * @param text The text to be shown informatively.
 */
#define winfo(qobj, text) wlog(qobj)->info(text)

/**
 * @brief Quickly grabs a logger and send text to error output.
 * @param qobj An object based on QObject.
 * @param text The text to be shown as an error message.
 */
#define werr(qobj, text) wlog(qobj)->error(text)

/**
 * @brief Quickly grabs a logger and send text to fatal output.
 * @param qobj An object based on QObject.
 * @param text The text to be shown as an fatal message.
 */
#define wfatal(qobj, text) wlog(qobj)->fatal(text)

/**
 * @brief Quickly grabs a logger and send text to trace output.
 * @param qobj An object based on QObject.
 * @param text The text to be shown as an trace message.
 */
#define wtrace(qobj, text) wlog(qobj)->trace(text)

#endif /* WINTERMUTE_LOGGING_HPP */
