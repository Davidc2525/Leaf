#include <stdio.h>
#include <iostream>
#include "../../../tree.hpp"
#include "../../registers/register_manager.hpp"
#include <fstream>
using namespace std;

namespace CODE_GENERATOR
{
namespace x86_64
{

void randomString(int size, char *output);
string *_to_reg_(int r, int offset);
string *_to_reg_(int r);



class x86_64Linux : public AstHandler
{
public:
    AST CreatePrintNode(AST ex);
    NODE *CreateStatementBlock();
    void StatementBlockAdd(AST BS, AST S);
    AST AssignmentNode(AST left, AST right);
    NODE *FuncDeclNode(string *name);
    AST FuncCall(AST f_node, bool);
    //AST ProcCall(AST f_node);

    virtual AST MemberNode(AST obj, AST propery);

    // void FuncCallAddArg(AST BS, AST S);
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
    // Creates an empty node

    AST CreateIfNode(AST);
    AST CreateIfElseNode(AST, AST);

    /**
     * 
     * Comienza la generacion de codigo a partir de el AST recolectado.
     * FALTA ACOMODAR
     * 
    */
    void GenerateCode(AST A, ExternSymbols *, StringLiterals *);
};
} // namespace x86_64
} // namespace CODE_GENERATOR
