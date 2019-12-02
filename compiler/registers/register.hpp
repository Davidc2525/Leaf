

#ifndef _REGISTER_H_
#define _REGISTER_H_
#include <iostream>
using namespace std;

class Register
{

public:
    Register();
    Register(int id,string*name, int bits,string*word);
    int id;
    string *name;
    int bits = 0;
    string *word;

    const char* getName();
    const char* dump();
}; 

#endif