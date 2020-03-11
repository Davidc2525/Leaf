

#include "symbol_table.hpp"

int symbol_table::get_id(Struct *type, int ac) 
{
    int id = ac;
    //cout<<"kind "<<type->kind<<endl;
    if (type->elemType != NULL)
    {
        id = get_id(type->elemType, id) + type->kind+id;
    }
    else
    {
        id+= type->kind;
    }

    return id;
}

//tab
//
int symbol_table::type_id = 20;
int symbol_table::globals = 0;
Scope *symbol_table::curScope = NULL;    // current top scope
int symbol_table::curLevel = NULL;       // nesting level of current scope
Struct *symbol_table::boolType = NULL;   // predefined types
Struct *symbol_table::byteType = NULL;   // predefined types
Struct *symbol_table::intType = NULL;    // predefined types
Struct *symbol_table::charType = NULL;   // predefined types
Struct *symbol_table::floatType = NULL;  // predefined types
Struct *symbol_table::doubleType = NULL; // predefined types
Struct *symbol_table::stringType = NULL;
Struct *symbol_table::nullType = NULL;
Struct *symbol_table::noType = NULL;

int symbol_table::cm_sp = 0;
Obj **symbol_table::currMethodStack = NULL;

Obj *symbol_table::chrObj = NULL; // predefined objects
Obj *symbol_table::ordObj = NULL;
Obj *symbol_table::lenObj = NULL;
Obj *symbol_table::noObj = NULL;

Obj *symbol_table::insert(int kind, string *name, Struct *type)
{
    Obj *obj = new Obj(kind, name, type);
    //cout << "; insert st, obj kind: " << kind << ", name " << *name << ", type kind: " << type->kind << ", level: " << curLevel << endl;
    if (type->kind == Struct::Arr) 
    {
       // cout << ";  - elemtype kind: " << type->elemType->kind << endl;
    }

    if (kind == Obj::Var)
    {
        //cout << ";current adr " << curScope->offset << " nvars " << curScope->nVars << endl;
        obj->adr = curScope->offset;
        curScope->nVars++;
        curScope->offset += Struct::sizes[type->kind];

        obj->level = curLevel;

        if (curLevel == 0)
        {
            globals = globals + 1;
            ;
        }
    }

    Obj *p = curScope->locals;
    Obj *last = NULL;

    while (p != NULL)
    {
        //if (*p->name == *name)
            //cout << "; " << *name << " declared twice" << endl;
        last = p;
        p = p->next;
    }

    if (last == NULL)
    {
        curScope->locals = obj;
    }
    else
    {
        last->next = obj;
    }
    // cout<<" inserted obj: "<<obj->name<<", p: "<<obj<<endl;
    return obj;
}

Obj *symbol_table::find(string *name)
{
    /*Obj *obj;
    Scope *scope;
    scope = curScope;
    while (scope != NULL) {  // for all open scopes
        obj = scope->locals;
        while (obj != NULL) {  // for all objects in this scope
            if (obj->name == name){
                 //cout<<" find obj: "<<obj->name<<", p: "<<obj<<endl;
                 return obj;}
            obj = obj->next;
        }
        scope = scope->outer;
    }*/
    bool ownScope = true;
    for (Scope *s = curScope; s != NULL; (s = s->outer, ownScope = false))
    {
        for (Obj *p = s->locals; p != NULL; p = p->next)
        {
            if (*p->name == *name)
            {
                /*cout<<"; find st, obj kind: "<<p->kind<<", name "<<*p->name<<", type kind: "<<p->type->kind<<endl;
                if(p->type->kind == Struct::Arr){
                    cout<<";  - elemtype kind: "<<p->type->elemType->kind<<endl;

                }*/
                //cout<<"; "<<*p->name<<" no pertenece a este ambito."<<endl;
                return p;
            }
        }
    }
    cout << "; " << *name << " is undeclared" << endl;
    return noObj;
};

Obj *symbol_table::findField(string *name, Struct *t)
{

    for (Obj *p = t->fields; p != NULL; p = p->next)
    {
        if (*p->name == *name)
            return p;
    }

    cout << "; " << *name << " field is undeclared" << endl;
    return symbol_table::noObj;
};

void symbol_table::openScope()
{
    Scope *s = new Scope();
    s->level = curLevel;
    s->outer = curScope;
    curScope = s;
    curLevel++;
};

void symbol_table::closeScope()
{
    curScope = curScope->outer;
    curLevel--;
};

symbol_table::symbol_table()
{

    init();
}

//

Obj *symbol_table::currMethod()
{
    return currMethodStack[cm_sp - 1];
}
void symbol_table::pushCurrMethod(Obj *m)
{
    currMethodStack[cm_sp++] = m;
}

Obj *symbol_table::popCurrMethod()
{
    return currMethodStack[cm_sp--];
}

//
void symbol_table::init()
{
    cout << "; iniciando tabla de simbolos" << endl;
    //PROCESS::Native * n = PROCESS::Native::getInstance();
    //Obj *o;
    cm_sp = 0;
    curScope = new Scope();
    curScope->outer = NULL;
    curLevel = 0;
    currMethodStack = (Obj **)malloc(sizeof(Obj *) * 100);

    //predeclared types
    boolType = new Struct(Struct::Bool);
    byteType = new Struct(Struct::Byte);
    intType = new Struct(Struct::Int);
    charType = new Struct(Struct::Char);
    floatType = new Struct(Struct::Float);
    doubleType = new Struct(Struct::Double);
    //stringType = new Struct(Struct::String);
    stringType = new Struct(Struct::Arr, charType);
    //string == char[]
    nullType = new Struct(Struct::Class);
    noType = new Struct(Struct::None);
    noObj = new Obj(Obj::Var, new string("???"), noType);

    //create predecared object
    insert(Obj::Type, new string(TYPES::_BOOOLEAN_), boolType);
    insert(Obj::Type, new string(TYPES::_BYTE_), byteType);
    insert(Obj::Type, new string(TYPES::_INT_), intType);
    insert(Obj::Type, new string(TYPES::_FLOAT_), floatType);
    insert(Obj::Type, new string(TYPES::_DOUBLE_), doubleType);
    insert(Obj::Type, new string(TYPES::_STRING_), stringType);
    insert(Obj::Type, new string(TYPES::_CHAR_), charType);
    insert(Obj::Con, new string(TYPES::_NULL_), nullType);
    insert(Obj::Type, new string("void"), noType);
}
