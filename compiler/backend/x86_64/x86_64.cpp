

#include "x86_64.hpp" 

namespace CODE_GENERATOR
{
namespace x86_64
{

//ofstream cout("pro.asm"); 

/**/ void randomString(int size, char *output) // pass the destination size and the destination itself
{

    srand(time(NULL)); // seed with time

    char src[size];
    size = rand() % size; // this randomises the size (optional)

    src[size] = '\0'; // start with the end of the string...

    // ...and work your way backwards
    while (--size > -1)
        src[size] = (rand() % 94) + 32; // generate a string ranging from the space character to ~ (tilde)

    strcpy(output, src); // store the random string
};
bool StructEq(Struct *o1, Struct *o2)
{
    if (o1->elemType != NULL && o2->elemType != NULL)
    {
        if (o1->kind == o2->kind)
        {
            StructEq(o1->elemType, o2->elemType);
        }
        else
        {
            return false;
        }
    }
    else
    {
        if (o1->elemType == NULL && o2->elemType != NULL)
        {
            return false;
        }
        else if (o2->elemType == NULL && o1->elemType != NULL)
        {
            return false;
        }
        return o1->kind == o2->kind;
    }
}

class BINOPNODE : public EXPR
{
    void load(int);
    void operation(optypes, int);
    void loadreg(int R) { load(R); }
};

class DeclFuncNode : public BLOCK
{
public:
    DeclFuncNode(string *n)
    {
        stms = (AST *)malloc(sizeof(AST) * 1000);
        name = n;
    };
    virtual void add(AST E);
    virtual void load(int R);
    virtual void lea(int R);
    virtual void store(int R);

    virtual void operation(optypes O, int R);
    virtual void loadreg(int R)
    {
        load(R);
    }
    AST *stms;
    int s = 0;
    string *name;
    Obj *aa = NULL;
};
void DeclFuncNode::add(AST E) {}
void DeclFuncNode::load(int R)
{
    RegisterProvider *rp = RegisterManager::getInstance()->getProviderBy(64);

    loadOpt->effectivea->parts.push_back(new EffectivePart(rp->reserve_by_size(128)->id));
    loadOpt->effectivea->parts.push_back(new EffectivePart(rp->reserve_by_size(32)->id));
    loadOpt->effectivea->parts.push_back(new EffectivePart(8, Num, Mul));
    loadOpt->effectivea->parts.push_back(new EffectivePart(8, Num, Mul));

    cout << "funcion: " << *name << " '" << loadOpt->effectivea->toString() << "'" << endl;
}
void DeclFuncNode::lea(int R) {}
void DeclFuncNode::store(int R) {}
void DeclFuncNode::operation(optypes op, int R)
{
    cout << "funcion: " << *name << endl;
}
//////////////////////////////

////////////////////////////////////////////////////

/*STM*/
class StatementBlockNode : public BLOCK
{
public:
    AST *stms;
    int s = 0;
    StatementBlockNode()
    {
        stms = (AST *)malloc(sizeof(AST) * 1000);
    }
    virtual void add(AST E);
    virtual void load(int R);
    virtual void operation(optypes O, int R){};
    virtual void loadreg(int R)
    {
        //dlog("LOAD REG IWTH LOAD " + to_string(R), 0);
        load(R);
    }
};
void StatementBlockNode::add(AST e)
{

    stms[s] = e;
    s++;
}
void StatementBlockNode::load(int R)
{

    for (int x = 0; x < s; x++)
    {
        //cout<<stms[x]<<endl;
        stms[x]->load(1);
        if (!stms[x]->obj->_extern)
            cout << endl;
    }
}

AST x86_64Linux::CreatePrintNode(AST ex) {}
AST x86_64Linux::CreateStatementBlock() { return new StatementBlockNode(); }
void x86_64Linux::StatementBlockAdd(AST BS, AST S)
{
    //cout << ";StatementBlockAdd " << BS << " " << S << endl;
    ((BLOCK *)BS)->add((STAT *)S);
}
AST x86_64Linux::AssignmentNode(AST left, AST right) {}
AST x86_64Linux::FuncDeclNode(string *name) { return new DeclFuncNode(name); }
AST x86_64Linux::FuncCall(AST f_node, bool) {}
//AST x86_64Linux::ProcCall(AST f_node) {}
// void FuncCallAddArg(AST BS, AST S){}
void x86_64Linux::SetValueAsReturnOfFunn(AST) {}
Obj *x86_64Linux::GetObjOfNode(NODE *n)
{
    return n->obj;
}
void x86_64Linux::SetObjOfNode(NODE *n, Obj *o)
{
    n->obj = o;
}
//expre
AST x86_64Linux::BinOpNode(optypes op, AST left, AST right) {}
// Creates an AST for the binary operation "left op right"
AST x86_64Linux::VarNode(string *name) {}
// Creates an AST for a variable factor with specified name
AST x86_64Linux::DesfRefVarNode(AST name) {}

AST x86_64Linux::RefVarNode(AST name) {}

AST x86_64Linux::ConstNode(int value) {}
AST x86_64Linux::ConstFloatNode(float value) {}
AST x86_64Linux::ConstStringNode(string *value) {}
// Creates an AST for a constant factor with specified value
AST x86_64Linux::ReturnNode(AST toReturn) {}
AST x86_64Linux::ReturnNode() {}
AST x86_64Linux::EmptyNode() {}
// Creates an empty node

AST x86_64Linux::CreateIfNode(AST) {}
AST x86_64Linux::CreateIfElseNode(AST, AST) {}

AST x86_64Linux::MemberNode(AST obj, AST propery)
{
}

/**
     * 
     * Comienza la generacion de codigo a partir de el AST recolectado.
     * FALTA ACOMODAR
     * 
    */
void x86_64Linux::GenerateCode(AST A, ExternSymbols *es, StringLiterals *sl)
{
    cout << ";-----------------------------" << endl;
    cout << "section .data" << endl;
    sl->dump();
    cout << "formato db \"%d\",10,0" << endl;
    cout << "formato_real db \"%f\",10,0" << endl;
    cout << "formato_string db \"%s\",0" << endl;
    cout << "formato_char db \"%c\",10,0" << endl;
    cout << "" << endl;
    cout << "section .text" << endl;
    cout << "global main" << endl;
    cout << "extern printf" << endl;
    es->dump();
    cout << "" << endl;

    A->load(1);
    printf("\n");
}
} // namespace x86_64
} // namespace CODE_GENERATOR