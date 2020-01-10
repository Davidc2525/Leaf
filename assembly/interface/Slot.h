

#ifndef __SLOT__
#define __SLOT__

namespace LEAF
{
namespace ASSEMBLY
{

enum Slot_kind
{
	section,
	label,
	operand,
	instruccion,
	directive
};
class SlotVisitor;
class Slot
{
public:
	//virtual const char *to_string() = 0;
	virtual Slot_kind kind() = 0;
	virtual void accept(SlotVisitor *) = 0;
};

} // namespace ASSEMBLY
} // namespace LEAF
#endif