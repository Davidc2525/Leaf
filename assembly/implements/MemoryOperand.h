


#ifndef _DOPERAND_
#define _DOPERAND_
#include "../assembly"
using namespace LEAF::ASSEMBLY;

namespace LEAF
{
namespace ASSEMBLY
{

enum load_mod
{
	add,
	sub,
	mul,
	div,
	mod
};

const char* enum_load_mod_to_str(load_mod m);

class _op_pair_
{
public:
	_op_pair_(){};
	_op_pair_(load_mod, Operand *);
	load_mod lm;
	Operand *ope;
};

class MemoryOperand : public Operand
{
	friend IntelVisitor; 
private:
	vector<_op_pair_ *> parts;

public:
	MemoryOperand();
	void add_operand(Operand *);
	void add_operand(load_mod, Operand *);
	//slock
	void accept(SlotVisitor*);
	//const char *to_string();
	Slot_kind kind();
};
} // namespace ASSEMBLY
} // namespace LEAF

#endif
