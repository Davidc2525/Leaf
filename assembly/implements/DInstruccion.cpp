

#include "DInstruccion.h"


DInstruccion::DInstruccion(const char *i)
{
	set_instruccion(i);
}
void DInstruccion::set_instruccion(const char *i)
{
	ins = i;
}
void DInstruccion::accept(SlockVisitor *v)
{
    v->visit(this);
}

void DInstruccion::add_operand(Operand *op, sizes s)
{
	this->operands.push_back(new op_pair(op, s));
}
void DInstruccion::add_operand(Operand *op)
{
	add_operand(op, none);
}
op_pair::op_pair(Operand *op, sizes s)
{
	this->operand = op;
	this->load_size = s;
}

Slock_kind DInstruccion::kind()
{
	return Slock_kind::instruccion;
}
