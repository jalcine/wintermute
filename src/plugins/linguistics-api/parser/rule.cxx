#include <data-api/syntax/bond.hpp>
#include <data-api/syntax/chain.hpp>
#include "rule.hxx"
#include <parser/binding.hpp>

using namespace Wintermute::Data::Linguistics;
using Wintermute::Linguistics::RulePrivate;

void RulePrivate::__init(Rule* p_rule)
{
    foreach (Syntax::Bond * bnd, m_chn.bonds()) {
        m_bndVtr.push_back ( (new Binding (*bnd, p_rule)));
    }
}
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
