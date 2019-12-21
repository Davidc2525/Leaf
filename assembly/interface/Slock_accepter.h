

#ifndef __SLOCK_ACCEPTER_H__
#define __SLOCK_ACCEPTER_H__

#include <map>
#include "Slock_visitor.h"

using namespace std;
namespace LEAF
{
namespace ASSEMBLY
{

class SlockAccepter
{
public:
    virtual void accept(SlockVisitor *) = 0;
};

} // namespace ASSEMBLY
} // namespace LEAF
#endif