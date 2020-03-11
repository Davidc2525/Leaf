


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
ASTU block_get(ASTU b, int index){
     ((BLOCK *)b)->nodos[index];
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
void set_obj(NODEU *node, Obj *o)
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
ASTU const_int_node(int value) { return new IntConst(value); }
ASTU const_float_node(float value) { return new FloatConst(value); }
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

ASTU new_throw(ASTU ex){
    return new ThrowStatementNode(ex);
}
ASTU block_try()
{
    return new TryStatementNode();
};
string *get_try_end_label(ASTU _try_)
{
    return ((TryStatementNode *)_try_)->get_end_label();
}
ASTU try_add_catch(ASTU _try_, ASTU _catch_)
{
    ((TryStatementNode *)_try_)->add_catch(_catch_);
    ((CatchStatementNode *)_catch_)->add_try_end_label(((TryStatementNode *)_try_)->get_end_label());
};
ASTU block_catch()
{
    return new CatchStatementNode();
};
string *get_catch_star_label(ASTU _catch_)
{
    return ((CatchStatementNode *)_catch_)->start_label;
}
void catch_set_adr_ex_var(ASTU _catch_,int index){
    ((CatchStatementNode *)_catch_)->var_ex = index;
}

void catch_set_ex_var(ASTU _catch_,ASTU ex_var){
    ((CatchStatementNode *)_catch_)->ex_var = ex_var;
}


} // namespace AST
} // namespace LEAF