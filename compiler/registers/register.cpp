
#include "register.hpp"

Register::Register(){

};
Register::Register(int id, string *name, int bits, string *word)
{
    this->id = id;
    this->name = name;
    this->bits = bits;
    this->word = word;
};

const char *Register::getName()
{
    return name->c_str();
}

const char *Register::dump()
{
    string *d_info = new string("Register@{");
    d_info->append("name: ");
    d_info->append(name->c_str());
    d_info->append(",bits: ");
    d_info->append(to_string(bits));
    d_info->append("}");
    return d_info->c_str();
}