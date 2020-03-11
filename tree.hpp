#ifndef _TREE_H_
#define _TREE_H_ 1
#include <iostream>
#include "compiler/symb_tab/symbol_table.hpp"
#define NOIMPLEMENT                        \
    cout << "Sin implementacion." << endl; \
    exit(1);
#define llog(s, e) \
                   \
    cout << s      \
         << endl;  \
                   \
    if (e != 0)    \
        exit(e);

#if (defined(DEBUG) && (DEBUG == true))
#define dlog(s, e) log(s, e)
#else
#define dlog(s, e)
#endif
#include <map>
#include <string.h> // this is for strcpy
#include <vector>

using namespace std;

/**
 * Operaciones de operandos
*/
enum optypes
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

/**
 * Indica a los nodos donde se cargara o almacenara el valor de la variable o constante
 * 
 * Por lo general si se carga una variable, se hara en registro
 * pero cuando son de otro tipo, por ejemplo: float, se debe cargar a a pila del FPU
 * para poder usarlo
*/
enum LoadStorePlace
{
    REG,
    MEMO,
    STACK,
    S_FPU //pila de la FPU
};

void randomString(int size, char *output);
string *_to_reg_(int r, int offset);
string *_to_reg_(int r);
bool sreq(Struct *, Struct *);

class ExternSymbols
{
public:
    ExternSymbols()
    {
        symbols = (string **)malloc(sizeof(string) * 1000);
    };
    void add(string *sym)
    {
        symbols[s] = sym;
        s++;
    };
    void dump()
    {
        cout << "; simbolos externos (" << s << ")" << endl;
        for (int i = 0; i < s; ++i)
        {
            cout << "extern " << *symbols[i] << endl;
        }
    };
    int s = 0;
    string **symbols;
};

class Rand_label
{

public:
    static string *newLabel(int size)
    {

        char *r = (char *)malloc(sizeof(char) * size);
        string tokens("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890_");
        for (int x = 0; x < size; x++)
        {

            r[x] = tokens[rand() % 63];
        }
        r[size] = '\0';
        return new string(r);
    };
};
class StringLiterals
{
public:
    StringLiterals(){};
    string *add(string *n_string)
    {

        string *key = new string("sl_");
        key->append(Rand_label::newLabel(s > 50 ? 20 : 10)->c_str());

        cout << "; ID " << *key << endl;

        //string nc(Rand_label::newLabel(20)->c_str());
        this->string_lits.insert(make_pair(*key, n_string));
        s++;
        return key;
    };
    void dump()
    {
        cout << "; strings literals (" << s << ")" << endl;
        for (const pair<string, string *> &p : string_lits)
        {
            std::cout << p.first << " db " << *p.second << ",0" << std::endl;
        }
    };

    int s = 0;

    map<string, string *> string_lits;
};

enum EffectiveType
{
   Reg,
   Num
};

enum EffectivePartLoadMode
{
    Sub,
    Add,
    Mul,
    Div
};
const char *modoToString(EffectivePartLoadMode mode);

class EffectivePart
{
public:
    EffectivePart();
    EffectivePart(int);
    EffectivePart(int, EffectiveType);
    EffectivePart(int, EffectivePartLoadMode);
    EffectivePart(int, EffectiveType,EffectivePartLoadMode);
    int load;
    EffectivePartLoadMode mode = Add;
    EffectiveType type = Reg;
};

class EffectiveAddr
{
public:
    EffectiveAddr(){};
    // bool withDir = false;
    bool withBp = false;
    //int count = 0;
    //int *regs = nullptr;
    //vector<EffectivePart *> regis;
    //vector<EffectivePart *> addrs;
    vector<EffectivePart *> parts;
    //int dir;
    /**
     * 
    */
    char *toString();
};

class LoadOpt
{
public:
    LoadOpt() { effectivea = new EffectiveAddr; };
    int reg = -1;
    EffectiveAddr *effectivea = nullptr;
    EffectiveAddr *secundary_effectivea = nullptr;
};

class StoreOpt
{
public:
    StoreOpt() { aefective = new EffectiveAddr; };
    int dir = -1;
    EffectiveAddr *aefective = nullptr;
};

class OpResult
{
    int reg = -1;
};
class NODE;
class STAT;
class BLOCK;
class EXPR;
typedef ::NODE *AST;
 
class NODE
{ //friend AST BinOpNode(optypes op, AST left, AST right);
    //friend class BINOPNODE;
    //friend class ASSIGNMENT;
public:
    Obj *obj = new Obj(0, new string(), new Struct());
    int type;
    NODE() { defined = 0; }
    virtual void load(int R) = 0;
    virtual void lea(int R){};
    virtual void store(int R){};
    // Generate code for loading value of a node into register R
    //protected:
    int value;   // value derived from this node
    int defined; // 1 if value is defined
    bool ifReturnOfFunc = false;

    LoadStorePlace loadPlace = LoadStorePlace::REG;
    LoadStorePlace storePlace = LoadStorePlace::MEMO;

    LoadOpt *loadOpt = new LoadOpt();
    StoreOpt *storeOpt = new StoreOpt;
    OpResult *opResult = new OpResult;

    virtual void operation(optypes O, int R) = 0;
    virtual void loadreg(int R) { dlog("LOAD REG EMPTY", 0); }
    virtual void leareg(int R) { dlog("LEA REG EMPTY", 0); }
    virtual void storereg(int R) { dlog("STORE REG EMPTY", 0); }
};

class EXPR : public NODE
{
};
class STAT : public EXPR
{
};
class BLOCK : public STAT
{
public:
    virtual void add(AST E)=0;
};

class AstHandler
{
public:
    virtual AST CreatePrintNode(AST ex){NOIMPLEMENT};
    virtual NODE *CreateStatementBlock(){NOIMPLEMENT};
    virtual void StatementBlockAdd(AST BS, AST S){NOIMPLEMENT};
    virtual AST AssignmentNode(AST left, AST right){NOIMPLEMENT};
    virtual NODE *FuncDeclNode(string *name){NOIMPLEMENT};
    virtual AST FuncCall(AST f_node, bool){NOIMPLEMENT};
    virtual AST ProcCall(AST f_node){NOIMPLEMENT};
    virtual void FuncCallAddArg(AST BS, AST S){NOIMPLEMENT};
    virtual void SetValueAsReturnOfFunn(AST){NOIMPLEMENT};
    virtual Obj *GetObjOfNode(NODE *n){NOIMPLEMENT};
    virtual void SetObjOfNode(NODE *n, Obj *o){NOIMPLEMENT};
    //expre
    virtual AST MemberNode(AST obj, AST propery){NOIMPLEMENT};

    virtual AST BinOpNode(optypes op, AST left, AST right){NOIMPLEMENT};
    // Creates an AST for the binary operation "left op right"
    virtual AST VarNode(string *name){NOIMPLEMENT};
    // Creates an AST for a variable factor with specified name
    virtual AST DesfRefVarNode(AST name){NOIMPLEMENT};

    virtual AST RefVarNode(AST name){NOIMPLEMENT};

    virtual AST ConstNode(int value){NOIMPLEMENT};
    virtual AST ConstFloatNode(float value){NOIMPLEMENT};
    virtual AST ConstStringNode(string *value){NOIMPLEMENT};
    // Creates an AST for a constant factor with specified value
    virtual AST ReturnNode(AST toReturn){NOIMPLEMENT};
    virtual AST ReturnNode(){NOIMPLEMENT};
    virtual AST EmptyNode(){NOIMPLEMENT};
    // Creates an empty node

    virtual AST CreateIfNode(AST){NOIMPLEMENT};
    virtual AST CreateIfElseNode(AST, AST){NOIMPLEMENT};

    /**
     * 
     * Comienza la generacion de codigo a partir de el AST recolectado.
     * 
    */
    virtual void GenerateCode(AST A, ExternSymbols *, StringLiterals *){NOIMPLEMENT};
};
/*
AST CreatePrintNode(AST ex);

NODE *CreateStatementBlock();
void StatementBlockAdd(AST BS, AST S);
AST AssignmentNode(AST left, AST right);

NODE *FuncDeclNode(string *name);
AST FuncCall(AST f_node, bool);
AST ProcCall(AST f_node);
void FuncCallAddArg(AST BS, AST S);
void SetValueAsReturnOfFunn(AST);

Obj *GetObjOfNode(NODE *n);
void SetObjOfNode(NODE *n, Obj *o);

//expre
AST BinOpNode(optypes op, AST left, AST right);
// Creates an AST for the binary operation "left op right"
AST VarNode(string *name);
// Creates an AST for a variable factor with specified name
AST DesfRefVarNode(AST name);

AST RefVarNode(AST name);

AST ConstNode(int value);
AST ConstFloatNode(float value);
AST ConstStringNode(string *value);
// Creates an AST for a constant factor with specified value
AST ReturnNode(AST toReturn);
AST ReturnNode();
AST EmptyNode();
// creates an empty node

AST CreateIfNode(AST);
AST CreateIfElseNode(AST, AST);

void GenerateCode(AST A, ExternSymbols *, StringLiterals *);
// Generates code from AST A
*/
#endif
