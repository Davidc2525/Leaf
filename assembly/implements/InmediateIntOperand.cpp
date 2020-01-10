

#include "InmediateIntOperand.h"

namespace LEAF
{
namespace ASSEMBLY
{
InmediateIntOperand::InmediateIntOperand(int value)
{
    this->value = value;
}
 
Slot_kind InmediateIntOperand::kind() { return Slot_kind::operand; }

void InmediateIntOperand::accept(SlotVisitor *v)
{
    v->visit(this);
}
} // namespace ASSEMBLY
} // namespace LEAF