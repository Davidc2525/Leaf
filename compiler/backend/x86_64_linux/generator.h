/**
 * 
 * Generador de instrucciones para el programa en ensamblador de salida.
 * 
 * Traduce el AST en instrucciones para generar el codigo del programa
 * ensamblador
 * 
 * @author Colmenares David [david25pcxtreme@gmail.com]
 * */

#ifndef __X86_64_VISITOR_GENERATOR__
#define __X86_64_VISITOR_GENERATOR__
#include "../../../assembly/assembly"
#include "../../registers/register.hpp"
#include "../../registers/register_manager.hpp"
#include "../ast_visitor.h"
#include <fstream>
using namespace LEAF::AST;
#define UPDATE_STATICS \
    visits_ins++;
/**
 * 
 * Implementacion de ast_visitor para generacion 
 * de codigo de x86_64 sobre plataforma linux (linux 64)
 * 
*/
class x86_64_linux_generator_visitor : public ast_visitor
{
    int visits_ins = 0;
    Assembly *assembly;
    ofstream *out_file;
    Register *reg;
    Register *op_reg = nullptr;
    RegisterProvider *rp;
    op_types ax;

public:
    x86_64_linux_generator_visitor(ast_visitor_args *args);
    virtual void init(ASTU f);
    virtual void visit(BlockStatemntNode *b);
    virtual void visit1(BlockStatemntNode *) { ; }
    virtual void visit2(BlockStatemntNode *) { ; }
    virtual void visit3(BlockStatemntNode *) { ; }
    virtual void visit4(BlockStatemntNode *) { ; }

    virtual void visit(FuncDeclStatementNode *);
    virtual void visit1(FuncDeclStatementNode *) { ; }
    virtual void visit2(FuncDeclStatementNode *) { ; }
    virtual void visit3(FuncDeclStatementNode *) { ; }
    virtual void visit4(FuncDeclStatementNode *) { ; }

    virtual void visit(BinOp *b);
    virtual void visit1(BinOp *);
    virtual void visit2(BinOp *);
    virtual void visit3(BinOp *) { ; }
    virtual void visit4(BinOp *) { ; }

    virtual void visit(Ident *b);
    virtual void visit1(Ident *);
    virtual void visit2(Ident *);
    virtual void visit3(Ident *) { ; }
    virtual void visit4(Ident *) { ; }

    virtual void visit(VarDecl *b) { ; }
    virtual void visit1(VarDecl *) { ; }
    virtual void visit2(VarDecl *) { ; }
    virtual void visit3(VarDecl *) { ; }
    virtual void visit4(VarDecl *) { ; }

    virtual void visit(Member *b) { ; }
    virtual void visit1(Member *) { ; }
    virtual void visit2(Member *) { ; }
    virtual void visit3(Member *) { ; }
    virtual void visit4(Member *) { ; }

    virtual void visit(AssignStatement *b);
    virtual void visit1(AssignStatement *) { ; }
    virtual void visit2(AssignStatement *) { ; }
    virtual void visit3(AssignStatement *) { ; }
    virtual void visit4(AssignStatement *) { ; }

    virtual void visit(Empty *b);
    virtual void visit1(Empty *) { ; }
    virtual void visit2(Empty *) { ; }
    virtual void visit3(Empty *) { ; }
    virtual void visit4(Empty *) { ; }
    int ident = 0;
};

#endif