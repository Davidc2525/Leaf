

#include "ast.hpp"
namespace LEAF
{
namespace AST
{
ASTU block_stat()
{
    return new BlockStatemntNode();
};
ASTU block_add(ASTU b, ASTU e)
{
    ((BLOCK *)b)->add(e);
}
ASTU func_decl(string *name)
{
    return new FuncDeclStatementNode(name);
}
ASTU func_call(ASTU f_node, bool) { ; }
ASTU procedure_call(ASTU f_node) { ; }
ASTU func_call_add_arg(ASTU b, ASTU s) { ; }
void set_value_as_return_of_fun(ASTU) { ; }
Obj *get_obj(ASTU node)
{
    return node->obj;
}  
void set_obj(NODEU* node, Obj *o)
{
    node->obj = o;
}
ASTU member_node(ASTU obj, ASTU property)
{
    return new Member(obj, property);
}
ASTU bin_op(op_types op, ASTU left, ASTU right)
{
    return new BinOp(left, op, right);
}
ASTU ident(string *name) { return new Ident(name); }
ASTU const_int_node(int value) { return new Number(value); }
ASTU const_float_node(float value) { ; }
ASTU const_double_node(float value) { ; }
ASTU const_string_node(string *value) { ; }
ASTU return_node(ASTU to_return) { ; }
ASTU return_node() { ; }
ASTU assign(ASTU left, ASTU right)
{
    return new AssignStatement(left, right);
}
ASTU empty_node() { return new Empty; }
ASTU print_node(ASTU) {}
ASTU if_node(ASTU) { ; }
ASTU if_else_node(ASTU) { ; }

bool sreq(Struct *o1, Struct *o2)
{
    if (o1->elemType != NULL && o2->elemType != NULL)
    {
        if (o1->kind == o2->kind)
        {
            sreq(o1->elemType, o2->elemType);
        }
        else
        {
            return false;
        }
    }
    else
    {
        if (o1->elemType == NULL && o2->elemType != NULL)
        {
            return false;
        }
        else if (o2->elemType == NULL && o1->elemType != NULL)
        {
            return false;
        }
        return o1->kind == o2->kind;
    }
}

} // namespace AST
} // namespace LEAF