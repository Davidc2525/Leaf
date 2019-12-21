
#include "expressions.hpp"

namespace LEAF
{
namespace AST
{
EXPRE::EXPRE(){};

BinOp::BinOp(ASTU l, op_types op, ASTU r)
{
    this->l = l;
    this->r = r;
    this->op = op;
    //obj->type=left->obj->type;
    this->obj->type = l->obj->type;

    //TODO: codigo para la coercion de tipos aritmeticos
}
ACCETPS_IMPL(BinOp)

Ident::Ident(string *name)
{
    this->name = name;
    
    this->obj->name = name;
}
void Ident::accept(ast_visitor *v)
{
    v->visit(this);
} 
void Ident::accept1(ast_visitor *v) { v->visit1(this); }
void Ident::accept2(ast_visitor *v) { v->visit2(this); }
void Ident::accept3(ast_visitor *v) { v->visit3(this); }
void Ident::accept4(ast_visitor *v) { v->visit3(this); }

Member::Member(){};
Member::Member(ASTU object, ASTU property)
{

    this->object = object;
    this->property = property;
}
ACCETPS_IMPL(Member)

ACCETPS_IMPL(Empty)

} // namespace AST
} // namespace LEAF