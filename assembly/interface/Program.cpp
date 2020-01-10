

#include "Program.h"

Program::Program(SlotVisitor *v)
{
    visitor = v;
    sections.insert(make_pair(Sections::text,
                              new Section(visitor)));
    sections.insert(make_pair(Sections::data,
                              new Section(visitor, Sections::data)));
    sections.insert(make_pair(Sections::bss,
                              new Section(visitor, Sections::bss)));
    sections.insert(make_pair(Sections::_extern,
                              new Section(visitor, Sections::_extern)));
    sections.insert(make_pair(Sections::global,
                              new Section(visitor, Sections::global)));
}
Program::Program()
    : Program(new IntelVisitor())
{
}

void Program::write(ostream &out)
{

    Sections secs[5] = {Sections::global,
                        Sections::_extern,
                        Sections::data,
                        Sections::bss,
                        Sections::text};

    for (int x = 0; x < 5; x++)
    {
        section(secs[x])->write(out);
    }
}

Section *Program::section(Sections s)
{
    map<Sections, Section *>::iterator found = sections.find(s);

    if (found != sections.end())
    {
        return found->second;
    }
    else
    {
        return nullptr;
    }
}
