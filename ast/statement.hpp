


/**
 * Contiene las sentencias 
*/

#ifndef __AST_STATEMENTS__
#define __AST_STATEMENTS__
#include <string>
#include <vector>
using namespace std;
#include "../compiler/symb_tab/symbol_table.hpp"
#include "ast.hpp"

class ast_visitor;

namespace LEAF
{
namespace AST
{

class STATEMENT : public NODEU
{
public:
    STATEMENT(){};
};

class BLOCK : public STATEMENT
{
public:
    virtual void add(ASTU E) = 0;
    int s = 0;
    vector<ASTU> nodos;
};

//
class VarDecl : public STATEMENT
{
public:
    VarDecl(string *name) { this->name = name; };
    string *name;
    ACCETPS_INTERFACE
};

class AssignStatement : public STATEMENT
{
public:
    AssignStatement(ASTU l, ASTU r);
    ASTU left;
    ASTU right;
    ACCETPS_INTERFACE
};

class BlockStatemntNode : public BLOCK
{
public:
    BlockStatemntNode(){};
    virtual void add(ASTU E);
    ACCETPS_INTERFACE
    //ASTU *stms;
};

class rand_label
{

public:
    static string *newLabel(int size)
    {

        char *r = (char *)malloc(sizeof(char) * size + 2);
        r[0] = '_';
        string tokens("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890_");
        for (int x = 1; x < size-1; x++)
        {

            r[x] = tokens[rand() % 63];
        }
        r[size - 1] = '_';
        r[size] = '\0';
        return new string(r);
    };
};

class ThrowStatementNode : public STATEMENT
{
public:
    ThrowStatementNode(ASTU ex);
    ACCETPS_INTERFACE
    ASTU child;
};

class TryStatementNode : public BLOCK
{
public:
    TryStatementNode();
    TryStatementNode(ASTU child);
    virtual void add(ASTU E);
    ACCETPS_INTERFACE
    /*catchs blocks*/
    void add_catch(ASTU c);
    string *get_end_label();
    vector<ASTU> _caths_;
    ASTU child;
    string *end_label;
    string *end_label_wout_pop;
};

class CatchStatementNode : public BLOCK
{
public:
    CatchStatementNode();
    virtual void add(ASTU E);
    ACCETPS_INTERFACE
    void add_try_end_label(string *);
    string *start_label;
    string *try_end_label;
    int var_ex = -1;
    ASTU ex_var;
};

class FuncDeclStatementNode : public BlockStatemntNode
{
public:
    FuncDeclStatementNode();
    FuncDeclStatementNode(string *name);
    ACCETPS_INTERFACE
    string *name;
    Obj *aa = NULL;
};
} // namespace AST
} // namespace LEAF
#endif