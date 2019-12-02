//cococpp gramatica.atg -frames ./frames -o ./parser
// g++ -std=c++11 main.cpp -o main

#include <string>
#include <iostream>
#include "tree.hpp"
#include "parser/Parser.h"
#include "parser/Scanner.h"
#include "threeFactory.hpp"
#include "compiler/symb_tab/symbol_table.hpp"
#include "AstHandlerLinuxX86_64.hpp"
#include "stdio.h"
#include <sys/timeb.h>
#include <wchar.h>
#include "compiler/registers/register_manager.hpp"
#include "compiler/registers/x86_64_register_provider.hpp"
#include "compiler/registers/x86_32_register_provider.hpp"

#define clog(s, e)     \
    cout << s << endl; \
    if (e != 0)        \
        exit(e);

#if (defined(DEBUG) && (DEBUG == true))
#define dlog(s, e) log(s, e)
#else
#define dlog(s, e)
#endif

using namespace std;

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

    ThreeFactory::getInstance()->addAstHandler(new AstHandler);
    int handler = ThreeFactory::getInstance()->addAstHandler(new CODE_GENERATOR::x86_64::AstHandlerLinuxX86_64);

    if (argc == 2)
    {
        wchar_t *fileName = coco_string_create(argv[1]);
        Scanner *scanner = new Scanner(fileName);
        Parser *parser = new Parser(scanner);
        parser->tab = new symbol_table();
        parser->ss = new ExternSymbols();
        parser->sl = new StringLiterals();

        try
        {
            parser->ast_h = ThreeFactory::getInstance()->getAstHandler(handler);
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
            dlog(" errores: " + to_string(parser->errors->count), 0)
        }

        coco_string_delete(fileName);

        delete parser;
        delete scanner;
    }
    else
        clog("-- No source file specified", 1) return 1;
}
