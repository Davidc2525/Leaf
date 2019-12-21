

#ifndef _ROPERAND_
#define _ROPERAND_
#include "../assembly"
#include <string>

using namespace std;
using namespace LEAF::ASSEMBLY;
namespace LEAF
{
namespace ASSEMBLY
{

class RegisterOperand : public Operand
{

public:
	string *reg;
	RegisterOperand(const char *);

	void accept(SlockVisitor*);
	//const char *to_string();
	Slock_kind kind();
};
} // namespace ASSEMBLY
} // namespace LEAF

#endif
