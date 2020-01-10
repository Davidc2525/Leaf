

#ifndef _INMEDIATE_INT_OPERAND_
#define _INMEDIATE_INT_OPERAND_
#include "../assembly"
#include <string>

using namespace std;
using namespace LEAF::ASSEMBLY;
namespace LEAF
{
namespace ASSEMBLY
{

class InmediateIntOperand : public Operand
{

public:
    int value;
    InmediateIntOperand(int);
    //slock
    void accept(SlotVisitor*);
    Slot_kind kind();
};
} // namespace ASSEMBLY
} // namespace LEAF

#endif
