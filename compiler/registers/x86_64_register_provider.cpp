#include "x86_64_register_provider.hpp"
#include <string.h>

/*
Register *  X86_64RegisterProvider::lessBussy(){
    return RegisterProvider::lessBussy();
}
*/

bool X86_64RegisterProvider::isBussy(int id)
{
    return RegisterProvider::isBussy(id);
};
Register *X86_64RegisterProvider::get(int id)
{
    return RegisterProvider::get(id);
};
Register *X86_64RegisterProvider::get_by_name(const char *name)
{
    return RegisterProvider::get_by_name2((char *)name);
};
Register *X86_64RegisterProvider::get_by_name3(string *name)
{
    return RegisterProvider::get_by_name(name->c_str());
};
Register *X86_64RegisterProvider::get_by_name2(char *name)
{
    return RegisterProvider::get_by_name2(name);
};
void X86_64RegisterProvider::free(int id)
{
    RegisterProvider::free(id);
};
void X86_64RegisterProvider::free_by_name(const char *name) { RegisterProvider::free_by_name(name); };
void X86_64RegisterProvider::free_by_name2(char *name) { RegisterProvider::free_by_name2(name); };
void X86_64RegisterProvider::free_by_name3(string *name) { RegisterProvider::free_by_name3(name); };

Register *X86_64RegisterProvider::reserve()
{
    //cout << ";reserve 64" << endl;
    return RegisterProvider::reserve();
};
Register *X86_64RegisterProvider::reserve_by_id(int id)
{
    return RegisterProvider::reserve_by_id(id);
};
Register *X86_64RegisterProvider::reserve_by_name(const char *name)
{
    return RegisterProvider::reserve_by_name(name);
};
Register *X86_64RegisterProvider::reserve_by_name2(char *name)
{
    return RegisterProvider::reserve_by_name2(name);
};
Register *X86_64RegisterProvider::reserve_by_name3(string *name) { RegisterProvider::reserve_by_name3(name); };

int X86_64RegisterProvider::getArc()
{
    return this->arc;
};

Register *X86_64RegisterProvider::reserve_by_size(int size)
{
    return RegisterProvider::reserve_by_size(size);
}

Register *X86_64RegisterProvider::get_by_size(int size)
{
    return RegisterProvider::get_by_size(size);
}

X86_64RegisterProvider::X86_64RegisterProvider()
{
    //this->registers = (Register **)malloc(sizeof(Register *) * 100);

    string *byte = new string("BYTE");   //8
    string *word = new string("WORD");   //16
    string *dword = new string("DWORD"); //32
    string *qword = new string("QWORD"); //64

    int counter = 0;

    registers_.push_back(new Register(counter++, new string("ah"), 8, byte));
    registers_.push_back(new Register(counter++, new string("al"), 8, byte));
    registers_.push_back(new Register(counter++, new string("bh"), 8, byte));
    registers_.push_back(new Register(counter++, new string("bl"), 8, byte));
    registers_.push_back(new Register(counter++, new string("ch"), 8, byte));
    registers_.push_back(new Register(counter++, new string("cl"), 8, byte));
    registers_.push_back(new Register(counter++, new string("dh"), 8, byte));
    registers_.push_back(new Register(counter++, new string("dl"), 8, byte));

    registers_.push_back(new Register(counter++, new string("ax"), 16, word));
    registers_.push_back(new Register(counter++, new string("bx"), 16, word));
    registers_.push_back(new Register(counter++, new string("cx"), 16, word));
    registers_.push_back(new Register(counter++, new string("dx"), 16, word));
    registers_.push_back(new Register(counter++, new string("si"), 16, word));
    registers_.push_back(new Register(counter++, new string("di"), 16, word));

    registers_.push_back(new Register(counter++, new string("eax"), 32, dword));
    registers_.push_back(new Register(counter++, new string("ebx"), 32, dword));
    registers_.push_back(new Register(counter++, new string("ecx"), 32, dword));
    registers_.push_back(new Register(counter++, new string("edx"), 32, dword));
    registers_.push_back(new Register(counter++, new string("esi"), 32, dword));
    registers_.push_back(new Register(counter++, new string("edi"), 32, dword));

    registers_.push_back(new Register(counter++, new string("rax"), this->arc, qword));
    registers_.push_back(new Register(counter++, new string("rbx"), this->arc, qword));
    registers_.push_back(new Register(counter++, new string("rcx"), this->arc, qword));
    registers_.push_back(new Register(counter++, new string("rdx"), this->arc, qword));
    registers_.push_back(new Register(counter++, new string("rsi"), this->arc, qword));
    registers_.push_back(new Register(counter++, new string("rdi"), this->arc, qword));

    registers_.push_back(new Register(counter++, new string("rbp"), this->arc, qword, false));
    registers_.push_back(new Register(counter++, new string("rsp"), this->arc, qword, false));

    registers_.push_back(new Register(counter++, new string("r8"), this->arc, qword));
    registers_.push_back(new Register(counter++, new string("r9"), this->arc, qword));
    registers_.push_back(new Register(counter++, new string("r10"), this->arc, qword));
    registers_.push_back(new Register(counter++, new string("r11"), this->arc, qword));
    registers_.push_back(new Register(counter++, new string("r12"), this->arc, qword));
    registers_.push_back(new Register(counter++, new string("r13"), this->arc, qword));
    registers_.push_back(new Register(counter++, new string("r14"), this->arc, qword));
    registers_.push_back(new Register(counter++, new string("r15"), this->arc, qword));

    string *no_word = new string("");
    registers_.push_back(new Register(counter++, new string("xmm0"), 128, no_word));
    registers_.push_back(new Register(counter++, new string("xmm1"), 128, no_word));
    registers_.push_back(new Register(counter++, new string("xmm2"), 128, no_word));
    registers_.push_back(new Register(counter++, new string("xmm3"), 128, no_word));
    registers_.push_back(new Register(counter++, new string("xmm4"), 128, no_word));
    registers_.push_back(new Register(counter++, new string("xmm5"), 128, no_word));
    registers_.push_back(new Register(counter++, new string("xmm6"), 128, no_word));
    registers_.push_back(new Register(counter++, new string("xmm7"), 128, no_word));
    registers_.push_back(new Register(counter++, new string("xmm8"), 128, no_word));
    registers_.push_back(new Register(counter++, new string("xmm9"), 128, no_word));
    registers_.push_back(new Register(counter++, new string("xmm10"), 128, no_word));
    registers_.push_back(new Register(counter++, new string("xmm11"), 128, no_word));
    registers_.push_back(new Register(counter++, new string("xmm12"), 128, no_word));
    registers_.push_back(new Register(counter++, new string("xmm13"), 128, no_word));
    registers_.push_back(new Register(counter++, new string("xmm14"), 128, no_word));

    reservers = new map<int, stack<int> *>;
    for (auto x = registers_.begin(); x != registers_.end(); x++)
    {
        reservers->insert(make_pair((*x)->id, new stack<int>));
    }
};