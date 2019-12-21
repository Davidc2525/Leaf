

#ifndef _INSTRUCCION_
#define _INSTRUCCION_

#include "Operand.h"
#include "sizes.h"
#include "Slock.h"

namespace LEAF
{
namespace ASSEMBLY
{

class Instruccion : public Slock
{
public:
	virtual void set_instruccion(const char *) = 0;
	virtual void add_operand(Operand *, sizes) = 0;
	virtual void add_operand(Operand *) = 0;
};

} // namespace ASSEMBLY
} // namespace LEAF

#endif
