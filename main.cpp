
/**
 * @author Colmenares David [david25pcxtreme@gmail.com]
 * */

#include <string.h>
#include <iostream>
#include "assembly/assembly"
#include "ast/ast.hpp"
#include "compiler/backend/x86_64_linux/generator.h"
#include "tree.hpp"
#include "parser/Parser.h"
#include "parser/Scanner.h"
#include "threeFactory.hpp"
#include "compiler/symb_tab/symbol_table.hpp"
//#include "AstHandlerLinuxX86_64.hpp"
#include "stdio.h"
#include <sys/timeb.h>
#include <wchar.h>
#include "compiler/registers/register_manager.hpp"
#include "compiler/registers/x86_64_register_provider.hpp"
#include "compiler/registers/x86_32_register_provider.hpp"
#include "compiler/backend/x86_64/x86_64.hpp"

#define clog(s, e)     \
    cout << s << endl; \
    if (e != 0)        \
        exit(e);

#if (defined(DEBUG) && (DEBUG == true))
#define dlog(s, e) clog(s, e)
#else
#define dlog(s, e)
#endif

using namespace std;
using namespace std::rel_ops;

string *label_of_function(Obj *f)
{

    string *label = new string();

    label->append(f->name->c_str());
    label->append("_");

    Obj *last = f->locals;

    while (last != NULL)
    {
        label->append("@");
        Struct *last_type = last->type;
        while (last_type->kind == Struct::Pointer || last_type->kind == Struct::Arr || last_type->kind == Struct::Func)
        {
            label->append(to_string(last_type->kind));
            label->append("_");
            last_type = last_type->elemType;
        }

        label->append(to_string(last_type->kind));
        label->append("");
        label->append("_");
        last = last->next;
    }

    Struct *last_type = f->type->elemType;
    label->append("#");
    while (last_type->kind == Struct::Pointer || last_type->kind == Struct::Arr || last_type->kind == Struct::Func)
    {
        label->append(to_string(last_type->kind));
        label->append("_");
        last_type = last_type->elemType;
    }
    label->append(to_string(last_type->kind));
    label->append(":");

    return label;
}

int main(int argc, char **argv)
{

    auto a = new Assembly(new Program());

    DInstruccion *i = new DInstruccion("mov");
    MemoryOperand *mo = new MemoryOperand;
    mo->add_operand(new RegisterOperand("rbp"));
    mo->add_operand(new InmediateIntOperand(8));
    i->add_operand(mo);
    i->add_operand(new RegisterOperand("rax"));

    a->get_program()->section(Sections::text)->add(new Label("main"));
    //for (int x = 0; x < 1000000; x++)
    a->get_program()->section(Sections::text)->add(i);
    ofstream *out = new ofstream("pro.asm");

    a->get_program()->write(*out);
    /*if(argc<2){ 
        log("falta el archivo de programa",1)
    }
    string filename(args[1]);
    Scanner * s = new Scanner((const unsigned char *)filename.c_str(),filename.length());
    Parser * p = new Parser(s);
    p->Parse();*/

    int x86_64_registers = RegisterManager::getInstance()->addProvider(
        new X86_64RegisterProvider());
    int x86_32_registers = RegisterManager::getInstance()->addProvider(
        new X86_32RegisterProvider());

    RegisterProvider *rp = RegisterManager::getInstance()->getProvider(x86_64_registers);
    try
    {
        Register *r = rp->reserve();
        rp->free(r->id);
    }
    catch (const char *ex)
    {
        cout << ex << endl;
        exit(1);
    }
    catch (int ex)
    {
        cout << ex << endl;
        exit(1);
    }

    //ThreeFactory::getInstance()->addAstHandler(new AstHandler);
    //int handler = ThreeFactory::getInstance()->addAstHandler(new CODE_GENERATOR::x86_64::AstHandlerLinuxX86_64);
    //int handler = ThreeFactory::getInstance()->addAstHandler(new CODE_GENERATOR::x86_64::x86_64Linux);

    if (argc == 2)
    {
        wchar_t *fileName = coco_string_create(argv[1]);
        Scanner *scanner = new Scanner(fileName);
        Parser *parser = new Parser(scanner);
        parser->tab = new symbol_table();
        //parser->ss = new ExternSymbols();
        //parser->sl = new StringLiterals();
        ast_visitor_args *args = new ast_visitor_args;
        args->ass = new Assembly(new Program()); 
        args->outfile = new string("tmp_pro_leaf_asm.asm");

        parser->data_section = args->ass->get_program()->section(Sections::data);
        parser->visitor_generator =
            new x86_64_linux_generator_visitor(args);

        try
        {
            //parser->ast_h = ThreeFactory::getInstance()->getAstHandler(handler);
        }
        catch (char const *ex)
        {
            cout << "Exception: " << ex << endl;
            exit(1);
        }

        parser->Parse();
        //test
        //cout<<*label_of_function(symbol_table::find(new string("david")))<<endl;
        //cout<<*label_of_function(symbol_table::find(new string("main")))<<endl;
        //test
        return 0;
        if (parser->errors->count == 0)
        {
        }
        else
        {
            dlog(" errores: " + to_string(parser->errors->count), 1)
        }

        coco_string_delete(fileName);

        delete parser;
        delete scanner;
    }
    else
        clog("-- No source file specified", 1) return 1;
}
