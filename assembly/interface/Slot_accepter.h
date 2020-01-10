

#ifndef __SLOCK_ACCEPTER_H__
#define __SLOCK_ACCEPTER_H__

#include <map>
#include "Slot_visitor.h"

using namespace std;
namespace LEAF
{
namespace ASSEMBLY
{

class SlotAccepter
{
public:
    virtual void accept(SlotVisitor *) = 0;
};

} // namespace ASSEMBLY
} // namespace LEAF
#endif