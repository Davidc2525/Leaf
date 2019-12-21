

#ifndef __LABEL__
#define __LABEL__

#include "../assembly"
#include <string>
using namespace std;

namespace LEAF
{
namespace ASSEMBLY
{

class Label : public Slock
{
private:

public:
    char *value;
    Label();
    Label(char *);
    //slock
   /// const char *to_string();
    Slock_kind kind();
    void accept(SlockVisitor *);
};

} // namespace ASSEMBLY
} // namespace LEAF

#endif