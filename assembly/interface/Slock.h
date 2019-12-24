

#ifndef __SLOCK__
#define __SLOCK__
//#include "Slock_accepter.h"
//#include "Slock_visitor.h"
namespace LEAF
{
namespace ASSEMBLY
{

enum Slock_kind
{
	section,
	label,
	operand,
	instruccion,
	directive
};
class SlockVisitor;
class Slock
{
public:
	//virtual const char *to_string() = 0;
	virtual Slock_kind kind() = 0;
	virtual void accept(SlockVisitor *) = 0;
};

} // namespace ASSEMBLY
} // namespace LEAF
#endif