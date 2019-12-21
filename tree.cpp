/*
 thrice -> hold up a light
 */
#include <stdio.h>
#include <iostream>
#include "tree.hpp"
#include "compiler/registers/register_manager.hpp"
#include <algorithm>
using namespace std;
int orp = 15; //offset register params

void randomString(int size, char *output) // pass the destination size and the destination itself
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

string *_to_reg_(int r, int offset)
{

    //return new string("R"+to_string(r));

    switch (r + offset)
    {
    case 1:
    {
        return new string("rax");
    }
    case 2:
    {
        return new string("rbx");
    }
    case 3:
    {
        return new string("rcx");
    }
    case 4:
    {
        return new string("rdx");
    }
    case 5:
    {
        return new string("rdi");
    }
    case 6:
    {
        return new string("rsi");
    }
    case 7:
    {
        return new string("r8");
    }
    case 8:
    {
        return new string("r9");
    }
    case 9:
    {
        return new string("r10");
    }
    case 10:
    {
        return new string("r11");
    }
    case 11:
    {
        return new string("r12");
    }
    case 12:
    {
        return new string("r13");
    }
    case 13:
    {
        return new string("r14");
    }
    case 14:
    {
        return new string("r15");
    }
    case 15:
    { //parametros
        return new string("rdi");
    }
    case 16:
    {
        return new string("rsi");
    }
    case 17:
    {
        return new string("rdx");
    }
    case 18:
    {
        return new string("rcx");
    }
    case 19:
    {
        return new string("r8");
    }
    case 20:
    {
        return new string("r9");
    }
    }
}

string *_to_reg_(int r)
{
    return _to_reg_(r, 0);
}

bool sreq(Struct *o1, Struct *o2)
{
    if (o1->elemType != NULL && o2->elemType != NULL)
    {
        if (o1->kind == o2->kind)
        {
            sreq(o1->elemType, o2->elemType);
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

EffectivePart::EffectivePart()
{
    this->mode = Add;
    this->type = Reg;
};
EffectivePart::EffectivePart(int load)
    : EffectivePart()
{
    this->load = load;
};
EffectivePart::EffectivePart(int load, EffectiveType type)
    : EffectivePart(load)
{
    this->mode = Add;
    this->type = type;
};
EffectivePart::EffectivePart(int load, EffectivePartLoadMode mod)
    : EffectivePart(load)
{
    this->mode = mod;
};
EffectivePart::EffectivePart(int load, EffectiveType type, EffectivePartLoadMode mod)
    : EffectivePart(load, type)
{
    this->mode = mod;
};
/////////////////////////
const char *modoToString(EffectivePartLoadMode mode)
{
    switch (mode)
    {
    case Add:
        return "+";
        break;
    case Sub:
        return "-";
        break;
    case Div:
        return "/";
        break;
    case Mul:
        return "*";
        break;

    default:
        break;
    }
};
bool compareEfectivePart(EffectivePart *p1, EffectivePart *p2)
{
    return (p1->mode < p2->mode);
}
char *EffectiveAddr::toString()
{
    RegisterProvider *rp = RegisterManager::getInstance()->getProviderBy(64);
    string *addr = new string();

    /**
     * TODO: delete
     * TODO: colocar el registro dependiendo de la arquitectura
    */
    if (withBp)
    {
        //  RegisterProvider *rp = RegisterManager::getInstance()->getProviderBy(64);

        addr->append(rp->get_by_name("rbp")->getName());
        addr->append("+");
    }

    if (!this->parts.empty())
    {
        //std::sort(regis.begin(), regis.end(), compareEfectivePart);

        for (int i = 0; i < parts.size(); i++)
        {

            if (i != 0)
                addr->append(modoToString(parts[i]->mode));
            if (parts[i]->type == Reg)
            {

                addr->append(rp->get(parts[i]->load)->getName());
            }
            else if (parts[i]->type == Num)
            {
                addr->append(to_string(parts[i]->load));
            }
        }
    }

    return (char *)addr->c_str();
}
