/**
 * @file wntrdata.hpp
 * @author Wintermute Development <wntr-devel@thesii.org>
 *
 * @section lcns Licensing
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
 *
 * @mainpage Wintermute Data (WntrData) Documentation
 * Wintermute relies on the aggregation and pooling of data resources
 * to make sense of the (possibly) ten of hundreds of nodes of information
 * it receives. There's dedicated systems to obtaining information regarding
 * syntax and linguistics for natural language processing (Wintermute::Data::Linguistics)
 * and ontological resources (Wintermute::Data::Ontology).
 * Currently, the language data system provides an abstract class system so that
 * plug-in developers can implement new and foreign sources of data for lexical
 * and syntactic binding. Plug-in developers are encouraged to focus on data sources
 * that introduce new sources of words aside from importing new syntactic information
 * for the fact the syntactic system still needs a bit more work before it becomes as
 * decently mature as the lexical system.
 *
 * @section N01 Obtaining Information
 * WntrData holds all of the lexical, syntactical, and ontological data for Wintermute.
 * Thus, the means of information obtaining (at least for lexical and syntactic) information
 * are similar, whereas ontological information resembles a vaguely knit representation of
 * of that shown by typical SPARQL interpretations and parsers.
 *
 * @subsection N011 Lexical Information
 * Lexical information is stored by using the Models system designed by Adrian Borucki <gentoolx@gmail.com>.
 * The syntactical information is also stored accordingly.
 * @see N012
 *
 * @subsection NO12 Semantic Information
 */
#ifndef WNTRDATA_HPP
#define WNTRDATA_HPP

#include "config.hpp"
#include "system.hpp"
#include "linguistics.hpp"
#include "lexical/lexical.hpp"
#include "syntax/syntax.hpp"
#include "dbus/adaptors.hpp"
#include "dbus/interfaces.hpp"

#endif /* __WNTRDATA_HPP__ */
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
