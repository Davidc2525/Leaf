

#include "Label.h"

namespace LEAF
{
namespace ASSEMBLY
{
void Label::accept(SlotVisitor *v)
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

Slot_kind Label::kind()
{
    return Slot_kind::label;
}

} // namespace ASSEMBLY
} // namespace LEAF