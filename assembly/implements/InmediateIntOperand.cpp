

#include "InmediateIntOperand.h"

namespace LEAF
{
namespace ASSEMBLY
{
InmediateIntOperand::InmediateIntOperand(int value)
{
    this->value = value;
}
 
Slock_kind InmediateIntOperand::kind() { return Slock_kind::operand; }

void InmediateIntOperand::accept(SlockVisitor *v)
{
    v->visit(this);
}
} // namespace ASSEMBLY
} // namespace LEAF