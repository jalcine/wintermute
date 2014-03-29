/**
 * vim: ft=cpp tw=78
 * Copyright (C) 2011 - 2014 Jacky Alciné <me@jalcine.me>
 *
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
 **/

#ifndef WINTERMUTE_PROCEDURE_CALL_HPP
#define WINTERMUTE_PROCEDURE_CALL_HPP

#include <functional>
#include <QtCore/QObject>
#include <QtCore/QVariant>
#include <QtCore/QSharedPointer>
#include <Wintermute/Events/call_filter.hpp>

namespace Wintermute
{
namespace Procedure
{
class Call;
class ReplyCall;
class CallPrivate;
/**
 * @class Wintermute::Procedure::Call
 * @see   Wintermute::Procedure::CallPointer
 * @see   Wintermute::Procedure::ReplyCall
 * @see   Wintermute::Procedure::MethodCall
 *
 * Calls are data types in Wintermute that represent the fabric of its
 * interprocess communication between different modules. They're used mainly
 * for procedure calling but can be used however deemed necessary. The most
 * common form of Call that's used is the `MethodCall` and `ReplyCall`.
 *
 * @note Throughtout Wintermute, the type `CallPointer` is used instead of
 *       directly handling with the pointer itself.
 */
class Call : public QObject
{
public:
  /**
   * @typedef CallbackSignature
   * @brief   Signature of a function to use when a Call gets a reply.
   */
  typedef std::function< void ( const QVariant & ) > CallbackSignature;

  /**
   * @typedef Pointer
   * @brief   Pointer type alias to handle sharing `Call` object.
   */
  typedef QSharedPointer<Call> Pointer;
  
private:
  friend class Events::CallFilter;

  Q_OBJECT
  Q_FLAGS ( Type Types )

  /**
   * @fn handleReply
   * @param reply The reply representing the information from an invocation.
   * @brief Invokes the callback with the reply's value.
   */
  void handleReply ( const Call::Pointer &reply ) const;

protected:
  Q_ENUMS ( Type )
  Q_DISABLE_COPY ( Call )
  QSharedDataPointer<CallPrivate> d;

  /**
   * @ctor
   * @brief Creates a new Call object wit the provided CallPrivate data.
   */
  Call ( const QSharedDataPointer<CallPrivate> &other_d );

  /**
   * @fn    invoke
   * @param data A list of QVariant variables.
   * @note  Work on making this method something of a superclass.
   * @see   Call::callback()
   *
   * Handles the invokation of this call. By default, this method shouldn't be
   * reached. This method, however, should never be reached.
   *
   */
  virtual QVariant invoke ( const QVariantList &data = QVariantList() );

public:
  /**
   * @enum Types
   *
   * Defines the potential types a Call can take in the procedural system. In
   * order to keep things clean, you can add your own types only if it's over
   * Call::TypeUser (this was intentional). Avoid using 
   */
  enum Types
  {
    TypeUndefined  = 0x0000,  // Undefined call. Don't bother with.
    TypeInvocation = 0x0001,  // Represents a call that's to be invoked.
    TypeReply      = 0x0002,  // Represents a reply to an invoked call.
    TypeUser       = 0x0099   // User-defined calls offset.
  };
  Q_DECLARE_FLAGS ( Type, Types );

  /**
   * @ctor
   * @fn Call
   * @param parent Used for QObject hierarchy.
   * @note You typically don't want to create a new Call this way.
   */
  explicit Call ( QObject *parent );

  /**
   * @dtor
   * @fn ~Call
   */
  virtual ~Call ();

  /**
   * @fn id
   * @brief Returns the internal ID used for this call.
   */
  quint64 id() const;

  /**
   * @fn name
   * @brief Obtains the name of the call.
   */
  QString name() const;

  /**
   * @fn type
   * @brief Obtains the type of call this is.
   */
  Type type() const;

  /**
   * @fn toString
   * @brief Generates a string representation of this call.
   */
  QString toString() const;

  /**
   * @fn recipient
   * @brief Obtains the name of the module to recieve this message.
   */
  QString recipient() const;

  /**
   * @fn isValid
   * @brief Determines if this Call's data is miniminally viable.
   */
  virtual bool isValid () const;

  /**
   * @fn setRecipient
   * @brief Changes the recipient of this Call.
   */
  void setRecipient ( const QString &moduleName );

  /**
   * @operator operator()
   * @brief Allow for functor-like capabilities for the Call object.
   */
  QVariant operator() ( const QVariantList &arguments = QVariantList() );

  /**
   * @fn setCallback
   * @brief Sets the callback to be invoked when a reply is received.
   */
  void setCallback ( const CallbackSignature &signature );

  /**
   * @fn clearCallback
   * @brief Wipes the known callback used for this Call.
   */
  void clearCallback ();

  /**
   * @fn    fromString
   * @brief Creates a Call from a QString.
   */
  static Call::Pointer fromString ( const QString &data );
};
} /* Procedure */
} /* Wintermute */

/***
 * @function wCallCheckFlag
 * @param callPtr A reference or shared pointer to a Call.
 * @param flag    The flag to check for.
 */
bool wCallCheckFlag (const Wintermute::Procedure::Call& call, 
    const Wintermute::Procedure::Call::Types& flag);

Q_DECLARE_OPERATORS_FOR_FLAGS ( Wintermute::Procedure::Call::Type )
#endif /* WINTERMUTE_PROCEDURE_CALL_HPP */
