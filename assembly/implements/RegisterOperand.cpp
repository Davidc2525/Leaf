

#include "RegisterOperand.h"

namespace LEAF
{
namespace ASSEMBLY
{

void RegisterOperand::accept(SlockVisitor *v)
{
    v->visit(this);
}

RegisterOperand::RegisterOperand(const char *r)
{
	reg = new string(r);
}

Slock_kind RegisterOperand::kind()
{
	return Slock_kind::operand;
}

} // namespace ASSEMBLY
} // namespace LEAF