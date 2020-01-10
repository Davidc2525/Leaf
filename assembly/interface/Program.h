

#ifndef __PROGRAM__
#define __PROGRAM__

#include <map>
#include "../assembly"
using namespace std;
namespace LEAF
{
namespace ASSEMBLY
{
class Program
{
private:
	SlotVisitor *visitor;
	map<Sections, Section *> sections;

public:
	Program();
	Program(SlotVisitor *v);
	void write(ostream &);
	Section *section(Sections);
};

} // namespace ASSEMBLY
} // namespace LEAF
#endif
