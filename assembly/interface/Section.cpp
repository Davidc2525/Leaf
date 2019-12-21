
#include "../assembly"
using namespace LEAF::ASSEMBLY;

Section::Section(SlockVisitor *v) { visitor = v; }

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