

#if !defined(_SYMBOL_TABLE_H)
#define _SYMBOL_TABLE_H

#include "type_table.h"
#include <iostream>
#include <map>
using namespace std;

class Obj;
class Struct;
class Scope;
class symbol_table;

class symbol_table
{
public:
    symbol_table();
    ~symbol_table();

    static int globals;
    //static PROCESS::Native * native; // current top scope
    static Scope *curScope;  // current top scope
    static int curLevel;     // nesting level of current scope
    static Struct *byteType; // predefined types
    static Struct *intType;  // predefined types
    static Struct *charType; // predefined types
    static Struct *boolType;
    static Struct *floatType;
    static Struct *doubleType;
    static Struct *stringType;
    static Struct *nullType;
    static Struct *noType;
    static Obj *chrObj; // predefined objects
    static Obj *ordObj;
    static Obj *lenObj;
    static Obj *noObj;

    static Obj *insert(int kind, string *name, Struct *type);
    static Obj *find(string *name);
    static Obj *findField(string *name, Struct *type);

    static int cm_sp;
    static Obj **currMethodStack;
    static Obj *currMethod();
    static void pushCurrMethod(Obj *m);
    static Obj *popCurrMethod();

    static void openScope();
    static void closeScope();
    static void init();
};
class Struct
{ //type
public:
    static const int
        None = 0,
        Int = 1, Float = 2, Double = 3, Byte = 13, //numbers
        String = 4,
        Char = 11,
        Arr = 5, Pointer = 12,
        Class = 6,
        Pack = 7, //TODO
        Bool = 8,
        Func = 9,
        Meth = 10;

    static map<int, int> sizes;
    int size = 0;

    int kind;                // None, Int, String, Arr, Class
    Struct *elemType = NULL; // Arr: element type
    int nFields;             // Class: number of fields
    Obj *fields = NULL;      // Class: list of fields

    Struct()
    {
        this->kind = 0;
    };
    Struct(int kind);
    Struct(int kind, Struct *elemType);
    Struct(int kind, Struct &elemType);
    //int get_size(int type);
    bool isRefType();
    bool equals(Struct *other);
    bool compatibleWith(Struct *other);
    bool assignableTo(Struct *dest);
    void assign(Struct *o);

    bool operator==(Struct s2)
    {
        return kind == s2.kind;
    }
    //string toString();
};

class Obj
{
public:
    static const int Con = 0, Var = 1, Type = 2, Meth = 3, Func = 4;

    Obj(int kind, string *name, Struct *type);
    Obj(){};

    int kind; // Con, Var, Type, Meth, Func
    bool annon = true;
    string *name;
    bool is_ref = false;

    Struct *type = NULL;
    Obj *next = NULL;

    /*----------------------------------------------------*/
    bool _extern = false; //Meth,Func: native
    //int  native_id = 0; //Meth,Func: native

    int val; // Con: value

    int adr = -1; // Var, Meth,Func: address
    int level;    // Var: 0 = global, 1 = local

    int nPars;   // Meth,Func: number of parameters
    Obj *locals; // Meth,Func: parameters and local objects
};

class Scope
{
public:
    Scope *outer; // to the next outer scope
    Obj *locals;  // to the objects in this scope
    int offset = 8;
    int nVars; // number of variables in this scope (for address allocation)
    int level = 0;
    string toString();
};

#endif