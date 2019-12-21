

#ifndef _X86_RP_H_
#define _X86_RP_H_

#include "register_provider.hpp"
#include <vector>
#include <list>

//#include <algorithm>
#ifdef __cpluspluss
extern "C"
{
#endif
    class X86_64RegisterProvider : public RegisterProvider
    {
        Register **registers;
        // vector<Register *> registers_;
        //Register*find(int id);
    public:
        virtual bool isBussy(int id);
        virtual Register *get(int);
        virtual Register *get_by_name(const char *);
        virtual Register *get_by_name2(char *);
        virtual Register *get_by_name3(string *);

        virtual void free(int);
        virtual void free_by_name(const char *);
        virtual void free_by_name2(char *);
        virtual void free_by_name3(string *);

        virtual Register *reserve();
        virtual Register *reserve_by_id(int);
        virtual Register *reserve_by_name(const char *);
        virtual Register *reserve_by_name2(char *);
        virtual Register *reserve_by_name3(string *);

        virtual int getArc();

        virtual Register *reserve_by_size(int);
        virtual Register *get_by_size(int);
        //virtual Register *lessBussy(); //trabajar con la heredada

        X86_64RegisterProvider();
        int arc = 64;
    };

#ifdef __cpluspluss
}
#endif

#endif