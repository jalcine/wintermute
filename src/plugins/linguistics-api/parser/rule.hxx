#include <QList>
namespace Wintermute
{
namespace Data
{
namespace Linguistics
{
namespace Syntax
{
class Chain;
}
}
}
namespace Linguistics
{

    class Rule;
class Binding;
using Wintermute::Data::Linguistics::Syntax::Chain;
typedef QList<Binding*> BindingList;
struct RulePrivate {
    void __init(Rule* p_rule);
    Data::Linguistics::Syntax::Chain m_chn;
    BindingList m_bndVtr;
};
}
}
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
