
#include "symbol_table.hpp"


map<int,int> Struct::sizes = {
    {None,0},
    {Int,8}, 
    {Float,8},
    {Double,8},
    {Byte,1},
    {String,8}, 
    {Char,1},
    {Arr,8},
    {Pointer,8},
    {Class,0},
    {Pack,0},
    {Bool,4},
    {Func,8},
    {Meth,8}
};


Struct::Struct(int kind){
    this->kind = kind;
    this->nFields = 0;
    this->fields = NULL;
    this->elemType = NULL;
}

Struct::Struct(int kind, Struct * elemType){
    this->kind = kind;
    this->elemType = elemType;
    this->nFields = 0;
    this->fields = new Obj();
}

Struct::Struct(int kind, Struct & elemType){
    this->kind = kind;
    this->elemType = &elemType;
    this->nFields = 0;
    this->fields = new Obj();
}

void Struct::assign(Struct * o){
    this->kind = o->kind;
    this->elemType = (o->elemType);
    this->nFields = o->nFields;
    this->fields = o->fields;
}

bool Struct::isRefType(){
    return kind == Class || kind == Arr || kind == String;
};
bool Struct::equals(Struct * other){
    if(kind == Arr){
        return other->kind == Arr && other->elemType == elemType;
    }else{
        return this->kind==other->kind;
    }
};
bool Struct::compatibleWith(Struct * other){
    return this->equals(other)
        || this == symbol_table::nullType && other->isRefType()
        || other == symbol_table::nullType && this->isRefType();
};
bool Struct::assignableTo(Struct * dest){
    return this->equals(dest)
        || this == symbol_table::nullType && dest->isRefType()
        || kind == Arr && dest->kind == Arr && dest->elemType == symbol_table::noType;
};

 
 