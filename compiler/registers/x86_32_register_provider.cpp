

#include "x86_32_register_provider.hpp"

bool X86_32RegisterProvider::isBussy(int id)
{
    return RegisterProvider::isBussy(id);
};
Register *X86_32RegisterProvider::get(int id)
{
    return RegisterProvider::get(id);
};
Register *X86_32RegisterProvider::get_by_name(const char *name)
{
    return RegisterProvider::get_by_name2((char *)name);
};
Register *X86_32RegisterProvider::get_by_name3(string *name)
{
    return RegisterProvider::get_by_name(name->c_str());
};
Register *X86_32RegisterProvider::get_by_name2(char *name)
{
    return RegisterProvider::get_by_name2(name);
};
void X86_32RegisterProvider::free(int id)
{
    RegisterProvider::free(id);
};
void X86_32RegisterProvider::free_by_name(const char *name) { RegisterProvider::free_by_name(name); };
void X86_32RegisterProvider::free_by_name2(char *name) { RegisterProvider::free_by_name2(name); };
void X86_32RegisterProvider::free_by_name3(string *name) { RegisterProvider::free_by_name3(name); };

Register *X86_32RegisterProvider::reserve()
{
    //cout << ";reserve 32" << endl;
    return RegisterProvider::reserve();
};
Register *X86_32RegisterProvider::reserve_by_id(int id)
{
    return RegisterProvider::reserve_by_id(id);
};
Register *X86_32RegisterProvider::reserve_by_name(const char *name)
{
    return RegisterProvider::reserve_by_name(name);
};
Register *X86_32RegisterProvider::reserve_by_name2(char *name)
{
    return RegisterProvider::reserve_by_name2(name);
};
Register *X86_32RegisterProvider::reserve_by_name3(string *name) { RegisterProvider::reserve_by_name3(name); };
int X86_32RegisterProvider::getArc()
{
    return this->arc;
};

X86_32RegisterProvider::X86_32RegisterProvider()
{
    string *byte = new string("BYTE");   //8
    string *word = new string("WORD");   //16
    string *dword = new string("DWORD"); //32
    //string *qword = new string("QWORD"); //64

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

    reservers = new map<int, stack<int> *>;
    for (auto x = registers_.begin(); x != registers_.end(); x++)
    {
        reservers->insert(make_pair((*x)->id, new stack<int>));
    }
};