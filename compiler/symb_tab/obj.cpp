
#include "symbol_table.hpp"

Obj::Obj(int kind, string *name, Struct *type)
{
    this->kind = kind;
    this->name = name;
    this->type = type;
}
