

#include "SectionSlot.h"

namespace LEAF
{
namespace ASSEMBLY
{

void SectionSlot::accept(SlotVisitor *v)
{
    v->visit(this);
}

SectionSlot::SectionSlot(Sections s)
{
    sect = s;
}

Slot_kind SectionSlot::kind()
{
    return Slot_kind::section;
}

} // namespace ASSEMBLY
} // namespace LEAF