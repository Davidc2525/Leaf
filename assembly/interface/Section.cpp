
#include "../assembly"

using namespace LEAF::ASSEMBLY;

Section::Section(SlockVisitor *v, Sections type)
{
    visitor = v;
    this->type = type;

    /**
     * Agregamos el slock para la seccion.
     * ejemplo:
     * 
     * section .data
     * ...
     * section .text
     * ...
     * 
    */
    bool show_title = false;
    CONF::Conf &conf = CONF::ConfManager::get().group("leaf.compiler.asm.section.");

    if (conf.get<bool>("show_title"))
    {
        if (type == Sections::text && conf.get<bool>("text.title.show"))
        {
            show_title = true;
        }
        if (type == Sections::data && conf.get<bool>("data.title.show"))
        {
            show_title = true;
        }
        if (type == Sections::bss && conf.get<bool>("bss.title.show"))
        {
            show_title = true;
        }
        if (type == Sections::global && conf.get<bool>("global.title.show"))
        {
            show_title = true;
        }
        if (type == Sections::_extern && conf.get<bool>("extern.title.show"))
        {
            show_title = true;
        }
    }

    if (show_title)
    {
        slocks.push_back(new SectionSlock(type));
    }
}
Section::Section(SlockVisitor *v)
    : Section(v, Sections::text) {}

void Section::write(ostream &out)
{
    for (auto i = slocks.begin(); i != slocks.end(); i++)
    {
        (*i)->accept(visitor);
        out << *visitor->get_src() << endl;
    }
}

void Section::add(Slock *s)
{
    slocks.push_back(s);
}
/* void SectionText::write() { cout << "write SectionText" << endl; }
void SectionData::write() { cout << "write SectionData" << endl; }
void SectionBss::write() { cout << "write SectionBss" << endl; }
void SectionExtern::write() { cout << "write SectionExtern" << endl; }
void SectionGlobal::write() { cout << "write SectionGlobal" << endl; }  */