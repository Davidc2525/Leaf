

#ifndef _REGISTER_PROVIDER_H_
#define _REGISTER_PROVIDER_H_

#include <iostream>
#include "register.hpp"
#include <vector>
#include <map>
#include <stack>
#include <string.h>

using namespace std;

#define NOIMPLEMENT                                         \
    cout << "RegisterProvider Sin implementacion." << endl; \
    exit(1);

/*
* Clase abstracta de proveedor de registros
*/
class RegisterProvider
{

public:
    vector<Register *> registers_;
    map<int, stack<int> *> *reservers;

    virtual bool isBussy(int id);

    virtual Register *get(int);
    virtual Register *get_by_name(const char *);
    virtual Register *get_by_name2(char *);
    virtual Register *get_by_name3(string *);

    virtual void free(int);
    virtual void free_by_name(const char *);
    virtual void free_by_name2(char *);
    virtual void free_by_name3(string *);
    virtual Register *lessBussy();

    virtual Register *reserve();
    virtual Register *reserve_by_id(int);
    virtual Register *reserve_by_name(const char *);
    virtual Register *reserve_by_name2(char *);
    virtual Register *reserve_by_name3(string *);

    /*nuevos*/
    virtual Register *reserve_by_size(int);
    virtual Register *get_by_size(int);

    virtual int getArc();
};

#endif