

#include "statement.hpp"

FuncDeclStatementNode::FuncDeclStatementNode() { name = new string(""); };
FuncDeclStatementNode::FuncDeclStatementNode(string *name)
{
    this->name = name;
    this->obj->name = name;
};
ACCETPS_IMPL(FuncDeclStatementNode)

ACCETPS_IMPL(VarDecl)

AssignStatement::AssignStatement(ASTU l, ASTU r)
{
    left = l;
    right = r;
};
ACCETPS_IMPL(AssignStatement)

ACCETPS_IMPL(BlockStatemntNode)
void BlockStatemntNode::add(ASTU e)
{
    nodos.push_back(e);
    //stms[s] = e;
    s++;
}
