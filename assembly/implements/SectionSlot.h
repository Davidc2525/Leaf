

#ifndef _SECTION_SLOT_
#define _SECTION_SLOT_
#include "../assembly"
#include "../../conf/conf.h"
#include <string>

using namespace std;
using namespace LEAF::ASSEMBLY;
namespace LEAF
{
namespace ASSEMBLY
{

class SectionSlot : public Slot
{

private:
public:
    Sections sect;
    SectionSlot(Sections);

    void accept(SlotVisitor *);
    //const char *to_string();
    Slot_kind kind();
};
} // namespace ASSEMBLY
} // namespace LEAF

#endif
