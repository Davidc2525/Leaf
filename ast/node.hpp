


#ifndef __AST_NODE__
#define __AST_NODE__

#include "../compiler/symb_tab/symbol_table.hpp"
//#include "../compiler/backend/ast_visitor.h"
#include "../compiler/backend/accepter.h"
#include <string>

using namespace std;

namespace LEAF
{
namespace AST
{

class NODEU : public ::accepter
{
public:
    NODEU() { defined = 0; }

    Obj *obj = new Obj(0, new string(), new Struct());
    int type;
    int value;   // value derived from this node
    int defined; // 1 if value is defined
    bool ifReturnOfFunc = false;
};
} // namespace AST
} // namespace LEAF

#endif