

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
	SlockVisitor *visitor;
	map<Sections, Section *> sections;

public:
	Program();
	Program(SlockVisitor *v);
	void write(ostream &);
	Section *section(Sections);
};

} // namespace ASSEMBLY
} // namespace LEAF
#endif
