/*


#ifndef _INTEL_SYNTAX_PROGRAM_
#define _INTEL_SYNTAX_PROGRAM_

#include "../assembly"
#include <vector>
#include <string>
#include <fstream>
using namespace std;
namespace LEAF
{
namespace ASSEMBLY
{

class IntelSyntaxProgram : public Program
{
private:
    map<Sections, Section *> sections;

public:
    void write(ostream &); 
    Section *section(Sections);
    IntelSyntaxProgram();
    ~IntelSyntaxProgram();
};

} // namespace ASSEMBLY
}; // namespace LEAF

#endif*/