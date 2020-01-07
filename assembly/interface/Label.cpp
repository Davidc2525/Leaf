

#include "Label.h"

namespace LEAF
{
namespace ASSEMBLY
{
void Label::accept(SlockVisitor *v)
{
    v->visit(this);
}

Label::Label()
{
    value = "";
}
Label::Label(char *name)
{
    this->value = name;
}

Slock_kind Label::kind()
{
    return Slock_kind::label;
}

} // namespace ASSEMBLY
} // namespace LEAF