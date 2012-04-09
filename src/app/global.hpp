/***
 *  This file is part of Wintermute.
 *
 *  Copyright (C) 2012 Jacky Alciné <jackyalcine@gmail.com>
 *
 *  Wintermute is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Library General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 *  Wintermute is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Library General Public License for more details.
 *
 *  You should have received a copy of the GNU Library General Public License
 *  along with Wintermute.  If not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

#ifndef WINTERMUTE_GLOBAL_HPP
#define WINTERMUTE_GLOBAL_HPP

#include <QtGlobal>
#include <QDebug>
#include <app/config.hpp>

// Nifty debugging macros.
#define TRACE_MSG fprintf(stderr, __FUNCTION__     \
"() [%s:%d] here I am\n", \
__FILE__, __LINE__)
/**
 * @brief Allows a class to be defined as a singleton.
 */
#define WINTER_SINGLETON(Type) \
    private:\
        static Type* s_inst;\
        Type();\
    \
    public:\
        inline static Type* instance ( )\
        {\
            if (s_inst == 0)\
            {\
                s_inst = new Type;\
                qDebug() << "[WINTER_SINGLETON()] Created singleton instance of " #Type << ".";\
            }\
            return s_inst;\
        }\

#define WINTER_SINGLETON_DEFINE(Type) Type* Type::s_inst = 0;

#if defined(WNTR_EXPORT_SYMBOLS)
#define WNTR_EXPORT Q_DECL_EXPORT
#else
#define WNTR_EXPORT Q_DECL_IMPORT
#endif

/**
 * @def WNTR_DEPRECATED
 * @ingroup WintermuteMacros
 *
 * The WNTR_DEPRECATED macro can be used to trigger compile-time warnings
 * with newer compilers when deprecated functions are used.
 *
 * For non-inline functions, the macro gets inserted at front of the
 * function declaration, right before the return type:
 *
 * \code
 * WNTR_DEPRECATED void deprecatedFunctionA();
 * WNTR_DEPRECATED int deprecatedFunctionB() const;
 * \endcode
 *
 * For functions which are implemented inline,
 * the WNTR_DEPRECATED macro is inserted at the front, right before the return
 * type, but after "static", "inline" or "virtual":
 *
 * \code
 * WNTR_DEPRECATED void deprecatedInlineFunctionA() { .. }
 * virtual WNTR_DEPRECATED int deprecatedInlineFunctionB() { .. }
 * static WNTR_DEPRECATED bool deprecatedInlineFunctionC() { .. }
 * inline WNTR_DEPRECATED bool deprecatedInlineFunctionD() { .. }
 * \endcode
 *
 * You can also mark whole structs or classes as deprecated, by inserting the
 * WNTR_DEPRECATED macro after the struct/class keyword, but before the
 * name of the struct/class:
 *
 * \code
 * class WNTR_DEPRECATED DeprecatedClass { };
 * struct WNTR_DEPRECATED DeprecatedStruct { };
 * \endcode
 *
 * \note
 * WNTR_DEPRECATED cannot be used for constructors,
 * use WNTR__CONSTRUCTOR_DEPRECATED instead.
 */

#ifdef __cplusplus
# include <QtCore/qglobal.h>
# ifndef WNTR_DEPRECATED
#  ifdef WNTR_DEPRECATED_WARNINGS
#   define WNTR_DEPRECATED Q_DECL_DEPRECATED
#  else
#   define WNTR_DEPRECATED
#  endif
# endif
#endif

#ifndef WNTR_CONSTRUCTOR_DEPRECATED
# ifdef __GNUC__
#  if __GNUC__ == 3 && __GNUC_MINOR__ <= 3
/* GCC 3.3.x cannot handle Qt 4.1.2's definition of Q_DECL_CONSTRUCTOR_DEPRECATED */
#   define WNTR_CONSTRUCTOR_DEPRECATED
#  else
#   define WNTR_CONSTRUCTOR_DEPRECATED Q_DECL_CONSTRUCTOR_DEPRECATED
#  endif
# else
#  define WNTR_CONSTRUCTOR_DEPRECATED Q_DECL_CONSTRUCTOR_DEPRECATED
# endif
#endif

#endif /* WINTERMUTE_GLOBAL_HPP */
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
