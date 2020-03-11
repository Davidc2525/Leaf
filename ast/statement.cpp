


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

ACCETPS_IMPL(ThrowStatementNode)
ThrowStatementNode::ThrowStatementNode(ASTU ex)
{
    child = ex;
}

ACCETPS_IMPL(TryStatementNode)
TryStatementNode::TryStatementNode(ASTU child)
{
    this->child = child;
}
TryStatementNode::TryStatementNode()
{
    end_label = rand_label::newLabel(10);
    end_label_wout_pop = rand_label::newLabel(10);
    //end_label->append(":");
}
void TryStatementNode::add(ASTU e)
{
    nodos.push_back(e);
    s++;
}
void TryStatementNode::add_catch(ASTU e)
{
    _caths_.push_back(e);
}

string *TryStatementNode::get_end_label()
{
    return end_label;
}

ACCETPS_IMPL(CatchStatementNode)
CatchStatementNode::CatchStatementNode()
{
    start_label = rand_label::newLabel(10);
}
void CatchStatementNode::add(ASTU e)
{
    nodos.push_back(e);
    s++;
}
void CatchStatementNode::add_try_end_label(string *tel)
{
    try_end_label = tel;
}