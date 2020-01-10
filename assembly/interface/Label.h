

#ifndef __LABEL__
#define __LABEL__

#include "../assembly"
#include <string>
using namespace std;

namespace LEAF
{
namespace ASSEMBLY
{

class Label : public Slot
{
private:

public:
    char *value;
    Label();
    Label(char *);
    //slock
   /// const char *to_string();
    Slot_kind kind();
    void accept(SlotVisitor *);
};

} // namespace ASSEMBLY
} // namespace LEAF

#endif