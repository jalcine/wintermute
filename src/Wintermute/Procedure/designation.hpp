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

namespace Wintermute
{
namespace Procedure
{
struct Designation {
  quint64 pid = 0; ///< @brief The PID at which a Module resides.
  QString package = QString::null; ///< The package where the Module resides.
  QString domain = QString::null; ///< The domain where the Module resides.

  static const Definition Null; ///< @brief Helper field for null types.
  static const int MetaTypeId; ///< @internal

  /**
   * @brief Creates a new Definition
   * @param[in] quint64 The PID to use.
   * @param[in] QString The package of the module.
   * @param[in] QString The domain of the module.
   */
  explicit Definition(const quint64&, const QString& package,
                      const QString& domain);

  Definition(); ///< @brief Null constructor.

  /**
   * @brief Determines if this is a valid definition.
   * @retval boolean Whether or not this definition is valid.
   *
   * Checks if there's a value for the package and domain of the definition.
   * @note This will return true even if the PID is zero.
   */
  bool valid() const;

  /**
   * @brief Casts this Definition into a QString.
   * @retval A QString that can be used to represent this Definition.
   */
  operator QString() const;

  /**
   * @brief Equality operator.
   * @param[in] Definition definition.
   * @retval Determines if the provided Definition is equal to this
   *         Definition.
   */
  bool operator==(const Definition& definition) const;

  /**
   * @brief Assignment operator.
   * @param[in] Definition definition.
   */
  void operator=(const Definition& definition);

  /**
   * @brief Converts a JSON string into a Definition.
   * @param[in] QString The JSON data to be used.
   * @retval Definition The definition to be created.
   */
  static Definition fromString(const QString& string);

  /**
   * @brief Creates a new, valid Definition.
   * @param[in] QString The domain of the definition.
   * @param[in] QString The package of the definition.
   * @param[in] quint64 The PID of the definition.
   * @retval Definition The new Definition, when successful.
   * @retval Definition::Null A null, invalid Definition if invalid
   * parameters are used.
   */
  static Definition compose(const QString& domain, const QString& package,
                            const quint64& pid = 0);
};
}
}

#endif

#endif
