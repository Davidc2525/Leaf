#ifndef __ASSEMBLY__
#define __ASSEMBLY__
#include "../assembly"
namespace LEAF
{
namespace ASSEMBLY
{

class Assembly
{
public:
	Assembly()
	{
		
	};
	Assembly(Program *p)
	{
		set_program(p);
	};
	void set_program(Program *p)
	{
		this->program = p;
	}
	Program *get_program()
	{
		return program;
	};

private:
	Program *program;
};
} // namespace ASSEMBLY
} // namespace LEAF

#endif