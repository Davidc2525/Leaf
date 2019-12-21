//TODO esta clase tiene q eliminarce

#ifndef __SECTION_VISITOR_H__
#define __SECTION_VISITOR_H__

#include <map>
#include "Section.h"
using namespace std;
namespace LEAF
{
namespace ASSEMBLY
{
class SectionVisitor
{
public:
    virtual void visit(Section*);
};

} // namespace ASSEMBLY
} // namespace LEAF
#endif