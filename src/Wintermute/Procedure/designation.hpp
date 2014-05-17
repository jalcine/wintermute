/* vi: set ft=cpp: */
/**
 * @author Jacky Alciné <me@jalcine.me>
 * @copyright © 2014 Jacky Alciné <me@jalcine.me>
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

#ifndef WINTERMUTE_PROCEDURE_DESIGNATION_HPP
#define WINTERMUTE_PROCEDURE_DESIGNATION_HPP

#include <QtCore/QString>
#include <QtCore/QVariant>

namespace Wintermute
{
namespace Procedure
{
struct Designation {
  quint64 pid; ///< The PID at which a Module resides.
  QString package; ///< The package where the Module resides.
  QString domain; ///< The domain where the Module resides.

  static const Designation Null; ///< @brief Helper field for null types.
  static const int MetaTypeId; ///< @internal

  /**
   * @brief Creates a new Designation
   * @param[in] quint64 The PID to use.
   * @param[in] QString The package of the module.
   * @param[in] QString The domain of the module.
   */
  explicit Designation(const quint64&, const QString& package,
                       const QString& domain);

  Designation(); ///< @brief Null constructor.

  /**
   * @brief Determines if this is a valid designation.
   * @retval boolean Whether or not this designation is valid.
   *
   * Checks if there's a value for the package and domain of the designation.
   * @note This will return true even if the PID is zero.
   */
  bool valid() const;

  /**
   * @brief Determines if this is a null designation.
   * @retval boolean Whether or not this designation is null.
   */
  bool isNull() const;

  /**
   * @brief Determines if this is a local designation.
   * @retval boolean Whether or not this designation is local.
   */
  bool isLocal() const;

  /**
   * @brief Casts this Designation into a QString.
   * @retval A QString that can be used to represent this Designation.
   */
  operator QString() const;

  /**
   * @brief Equality operator.
   * @param[in] Designation designation.
   * @retval Determines if the provided Designation is equal to this
   *         Designation.
   */
  bool operator==(const Designation& designation) const;

  /**
   * @brief Inequality operator.
   * @param[in] Designation designation.
   * @retval Determines if the provided Designation is not equal to this
   *         Designation.
   */
  bool operator!=(const Designation& designation) const;


  /**
   * @brief Assignment operator.
   * @param[in] Designation designation.
   */
  void operator=(const Designation& designation);

  /**
   * @brief Converts a JSON string into a Designation.
   * @param[in] QString The JSON data to be used.
   * @retval Designation The designation to be created.
   */
  static Designation fromString(const QString& string);

  /**
   * @brief Creates a new, valid Designation.
   * @param[in] QString The domain of the designation.
   * @param[in] QString The package of the designation.
   * @param[in] quint64 The PID of the designation.
   * @retval Designation The new Designation, when successful.
   * @retval Designation::Null A null, invalid Designation if invalid
   * parameters are used.
   */
  static Designation compose(const QString& domain, const QString& package,
                             const quint64& pid = 0);
};
}
}

///< @internal Adds Designation as a Qt meta-type.
Q_DECLARE_METATYPE(Wintermute::Procedure::Designation)

///< @internal Output stream handler for Designation.
QDataStream& operator <<(QDataStream& ,
                         const Wintermute::Procedure::Designation& );

///< @internal Input stream handler for Designation.
QDataStream& operator >>(QDataStream& ,
                         Wintermute::Procedure::Designation& );


#endif
