/**
 * Contiene las expreciones 
*/

#ifndef __AST_EXPRESSIONS__
#define __AST_EXPRESSIONS__
#include "../compiler/symb_tab/symbol_table.hpp"
#include "ast.hpp"
#include <string>

using namespace std;

#define EXPRE_TYPE(NAME) class NAME : public EXPRE

namespace LEAF
{
namespace AST
{

class EXPRE : public NODEU
{
public:
    //Obj *obj = new Obj(0, new string(), new Struct());
    int type;
    EXPRE();
};

EXPRE_TYPE(BinOp)
{
public:
    BinOp(ASTU l, op_types op, ASTU r);
    ASTU l, r;
    op_types op;
    ACCETPS_INTERFACE
};

EXPRE_TYPE(Ident)
{
public:
    //Obj *obj = new Obj(0, new string(), new Struct());
    Ident(string * name);
    string *name;
    ACCETPS_INTERFACE
};

EXPRE_TYPE(Member)
{
public:
    Member();
    Member(ASTU object, ASTU property);
    ASTU object;
    ASTU property;
    ACCETPS_INTERFACE
};

EXPRE_TYPE(Empty)
{

public:
    Empty(){};
    ACCETPS_INTERFACE
};

} // namespace AST
} // namespace LEAF

#endif