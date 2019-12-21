

#include "MemoryOperand.h"

const char *LEAF::ASSEMBLY::enum_load_mod_to_str(load_mod m)
{
	switch (m)
	{
	case add:
		return "+";
		break;
	case sub:
		return "-";
		break;
	case mul:
		return "*";
		break;
	case div:
		return "/";
		break;
	case mod:
		return "%";
		break;

	default:
		break;
	}
}
void MemoryOperand::accept(SlockVisitor *v)
{
    v->visit(this);
}

MemoryOperand::MemoryOperand()
{
}

void MemoryOperand::add_operand(load_mod m, Operand *ope)
{
	parts.push_back(new _op_pair_(m, ope));
}

void MemoryOperand::add_operand(Operand *ope)
{
	add_operand(load_mod::add, ope);
}

Slock_kind MemoryOperand::kind()
{
	return Slock_kind::operand;
}

_op_pair_::_op_pair_(load_mod m, Operand *ope)
{
	lm = m;
	this->ope = ope;
}
