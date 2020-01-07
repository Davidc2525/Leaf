

#ifndef __AST__
#define __AST__
#include "../compiler/symb_tab/symbol_table.hpp"
namespace LEAF
{
namespace AST
{
enum op_types
{
    Store,
    Load,
    Plus,
    Minus,
    Times,
    Slash,
    Mod,
    And,
    Or,
    BitAnd,
    BitOr,
    BitXor,
    BitShiftL,
    BitShiftR,
    Eq,
    Neq,
    Less,
    LessEq,
    Gtr,
    GtrEq,
    Lea
};

class NODEU;
class Empty;
class VarDecl;
class Number;
class Ident;
class BinOp;
class Member;
class CostInt;
class ConstFloat;
class ConstDlouble;
class AssignStatement;

class AssignStatement;
class BlockStatemntNode;
class FuncDeclStatementNode;

#define ACCETPS_INTERFACE         \
    void accept(ast_visitor *v);  \
    void accept1(ast_visitor *v); \
    void accept2(ast_visitor *v); \
    void accept3(ast_visitor *v); \
    void accept4(ast_visitor *v);

#define ACCETPS_IMPL(NAME)                                  \
    void NAME::accept(ast_visitor *v) { v->visit(this); }   \
    void NAME::accept1(ast_visitor *v) { v->visit1(this); } \
    void NAME::accept2(ast_visitor *v) { v->visit2(this); } \
    void NAME::accept3(ast_visitor *v) { v->visit3(this); } \
    void NAME::accept4(ast_visitor *v) { v->visit3(this); }

/**
 * 
 * Unidad basica del arbol sintactico
 * Ast Unit
*/
typedef NODEU *ASTU;

} // namespace AST
} // namespace LEAF

#include "node.hpp"
#include "expressions.hpp"
#include "statement.hpp"
#include <map>
#include <string.h> // this is for strcpy
#include <vector>
#include <iostream>
namespace LEAF
{
namespace AST
{

//#include "../compiler/backend/accepter.h"
//#include "../compiler/symb_tab/symbol_table.hpp"
/**
 * Operaciones de operandos
*/

//
ASTU block_stat();
ASTU block_add(ASTU b, ASTU e);
ASTU func_decl(string *name);
ASTU func_call(ASTU f_node, bool);
ASTU procedure_call(ASTU f_node);
ASTU func_call_add_arg(ASTU b, ASTU s);
void set_value_as_return_of_fun(ASTU);
Obj *get_obj(ASTU node);
void set_obj(NODEU* node, Obj *o);
ASTU member_node(ASTU obj, ASTU property);
ASTU bin_op(op_types op, ASTU left, ASTU right);
ASTU ident(string *name);
ASTU const_int_node(int value);
ASTU const_float_node(float value);
ASTU const_double_node(float value);
ASTU const_string_node(string *value);
ASTU return_node(ASTU to_return);
ASTU return_node();
ASTU empty_node();
ASTU assign(ASTU left, ASTU right);
ASTU print_node(ASTU);
ASTU if_node(ASTU);
ASTU if_else_node(ASTU);

bool sreq(Struct *o1, Struct *o2);

} // namespace AST
} // namespace LEAF
#endif
