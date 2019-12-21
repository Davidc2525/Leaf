

#ifndef _DINSTRUCCION_
#define _DINSTRUCCION_
#include "../assembly"
#include <vector>
#include <string>
using namespace std;
using namespace LEAF::ASSEMBLY;

namespace LEAF
{
namespace ASSEMBLY
{

class op_pair
{
public:
	op_pair(Operand *op, sizes s);

	sizes load_size;
	Operand *operand;
};
class DInstruccion : public Instruccion
{
private:

public:
	string *ins;
	vector<op_pair *> operands;
	DInstruccion()
	{
		set_instruccion("xxx");
	};
	DInstruccion(const char *);
	void set_instruccion(const char *);
	void add_operand(Operand *ope);
	void add_operand(Operand *ope, sizes);

	//slock
	void accept(SlockVisitor*);
	//const char *to_string();
	Slock_kind kind();
};
} // namespace ASSEMBLY
} // namespace LEAF

#endif
