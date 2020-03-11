


/** 
 * @author Colmenares David [david25pcxtreme@gmail.com]
 * */

#ifndef _REGISTER_H_
#define _REGISTER_H_
#include <iostream>
using namespace std;

class Register
{

public:
    Register();
    Register(int id,string*name, int bits,string*word);
    Register(int id,string*name, int bits,string*word,bool reservable);
    int id;
    string *name;
    int bits = 0;
    string *word;
    bool reservable = true;

    const char* getName();
    const char* dump();
}; 

#endif