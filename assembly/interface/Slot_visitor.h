

#ifndef __SLOT_VISITOR_H__
#define __SLOT_VISITOR_H__

#include "../assembly"
using namespace std;
namespace LEAF
{
namespace ASSEMBLY
{
class SlotVisitor
{
public:
    virtual string *get_src() = 0;

    virtual void visit(SectionSlot *) = 0;
    virtual void visit(Label *) = 0;
    virtual void visit(MemoryOperand *) = 0;
    virtual void visit(RegisterOperand *) = 0;
    virtual void visit(InmediateIntOperand *) = 0;
    virtual void visit(DInstruccion *) = 0;
};

} // namespace ASSEMBLY
} // namespace LEAF
#endif