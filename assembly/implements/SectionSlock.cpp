

#include "SectionSlock.h"

namespace LEAF
{
namespace ASSEMBLY
{

void SectionSlock::accept(SlockVisitor *v)
{
    v->visit(this);
}

SectionSlock::SectionSlock(Sections s)
{
    sect = s;
}

Slock_kind SectionSlock::kind()
{
    return Slock_kind::section;
}

} // namespace ASSEMBLY
} // namespace LEAF