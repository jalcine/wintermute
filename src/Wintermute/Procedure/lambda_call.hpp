/**
 * vim: ft=cpp tw=78
 * Copyright (C) 2011 - 2013 Jacky Alciné <me@jalcine.me>
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

#ifndef WINTERMUTE_PROCEDURE_LAMBDA_CALL_HPP
#define WINTERMUTE_PROCEDURE_LAMBDA_CALL_HPP

#include <Wintermute/Procedure/MethodCall>
#include <Wintermute/Procedure/ModuleCall>

namespace Wintermute
{
namespace Procedure
{
/**
 * @brief Allows lambda functions to be invoked in Module objects.
 * @sa Wintermute::Procedure::Module
 */
class LambdaCall : public ModuleCall
{
  public:
    /// @brief Provides the short-hand signature for Call methods.
    typedef std::function<void(QVariant, const MethodCall&)> Signature;

    /**
     * @brief Crafts a new lambda-powered call.
     * @param[in] QString The name of the call.
     * @param[in] Module The module to be part of.
     * @param[in] Signature The lambda to be added.
     */
    explicit LambdaCall(const QString& name, const Module* module,
                        const Signature& lambda);

    virtual ~LambdaCall(); ///< @brief Destructor.

    /**
     * @brief Obtains the lambda that's to be invoked.
     * @retval Signature The lambda to be invoked, in a function pointer.
     */
    Signature lambda() const;

    /**
     * @brief Determines the validity of this LambdaCall.
     * @retval boolean The validness of this calll.
     */
    virtual bool valid() const;

    /**
     * @brief Updates the lambda to be invoked by this method.
     * @param[in] Signature The lambda to be used.
     */
    void setLambda(const Signature& lambda);

    /**
     * @brief Invokes the lambda held by this LambdaCall.
     * @param[in] QVariantList The arguments to use.
     * @param[in] MethodCall The method call that's invoked this call.
     */
    virtual void invoke (const QVariant& arguments, const MethodCall& call);

  private:
    Signature m_lambda;
};
}
}

#endif
