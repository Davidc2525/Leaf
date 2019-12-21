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
    int s = 0;
    vector<ASTU> nodos;
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