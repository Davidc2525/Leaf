


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

IntConst::IntConst(int value)
{
    this->value = value;
} 
ACCETPS_IMPL(IntConst)

FloatConst::FloatConst(float value)
{
    this->value = value;
    adr_label = rand_label::newLabel(10);
    
}
ACCETPS_IMPL(FloatConst)




Ident::Ident(string *name)
{
    this->name = name;

    this->obj->name = name;
}
ACCETPS_IMPL(Ident)

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