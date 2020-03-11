
/**
 * @author Colmenares David [david25pcxtreme@gmail.com]
 * */

#include "conf/conf.h"
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
//#include "compiler/feactures/class/class_handler.h"
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
//using namespace LEAF::FEACTURE::CLASS;

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

void prepare_default_conf()
{
    LEAF::CONF::Conf &conf = LEAF::CONF::ConfManager::get();

    //generator
    conf.set<char *>("leaf.compiler.generator.outfile", "test/leaf.asm");

    //sections; grupo: leaf.compiler.asm.section.
    conf.set<bool>("leaf.compiler.asm.section.show_title", true); //general
    conf.set<bool>("leaf.compiler.asm.section.text.title.show", true);
    conf.set<bool>("leaf.compiler.asm.section.data.title.show", true);
    conf.set<bool>("leaf.compiler.asm.section.bss.title.show", true);
    conf.set<bool>("leaf.compiler.asm.section.extern.title.show", true);
    conf.set<bool>("leaf.compiler.asm.section.global.title.show", true);
}

int main(int argc, char **argv)
{

    //prepare default global configuration
    prepare_default_conf();

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
        //args->outfile = new string("tmp_pro_leaf_asm.asm");

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
