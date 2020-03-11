


#include "RegisterOperand.h"

namespace LEAF
{
namespace ASSEMBLY
{

void RegisterOperand::accept(SlotVisitor *v)
{
    v->visit(this);
}

RegisterOperand::RegisterOperand(const char *r)
{
	reg = new string(r);
}

Slot_kind RegisterOperand::kind()
{
	return Slot_kind::operand;
}

} // namespace ASSEMBLY
} // namespace LEAF