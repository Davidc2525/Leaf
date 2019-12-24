

#ifndef _SECTION_SLOCK_
#define _SECTION_SLOCK_
#include "../assembly"
#include <string>

using namespace std;
using namespace LEAF::ASSEMBLY;
namespace LEAF
{
namespace ASSEMBLY
{

class SectionSlock : public Slock
{

private:
    
public:
    Sections sect;
    SectionSlock(Sections);

    void accept(SlockVisitor *);
    //const char *to_string();
    Slock_kind kind();
};
} // namespace ASSEMBLY
} // namespace LEAF

#endif
