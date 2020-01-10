

#include "IntelVisitor.h"

namespace LEAF
{
namespace ASSEMBLY
{

string *IntelVisitor::get_src() { return this->tmp; }

void IntelVisitor::visit(SectionSlot *l)
{
    string *s = new string("");

    switch (l->sect)
    {
    case Sections::text:
        s->append("section .text");
        break;
    case Sections::data:
        s->append("section .data");
        break;
    case Sections::_extern:
        s->append(";extern");
        break;
    case Sections::bss:
        s->append("section .bss");
        break;
    case Sections::global:
        s->append(";global");
        break;

    default:
        break;
    }
    this->tmp = s;
}
void IntelVisitor::visit(Label *l)
{
    string *label = new string(l->value);
    label->append(":");
    this->tmp = label;
}
void IntelVisitor::visit(MemoryOperand *l)
{
    string *_body = new string("[");
    int c = 0;
    for (auto x = l->parts.begin(); x != l->parts.end(); x++)
    {
        if (x != l->parts.begin())
            _body->append(enum_load_mod_to_str((*x)->lm));

        (*x)->ope->accept(this);
        _body->append(this->tmp->c_str());
    }
    _body->append("]");
    this->tmp = _body;
}
void IntelVisitor::visit(RegisterOperand *l)
{
    this->tmp = l->reg;
}
void IntelVisitor::visit(InmediateIntOperand *l)
{
    this->tmp = new string(to_string(l->value).c_str());
}
void IntelVisitor::visit(DInstruccion *l)
{
    string *out = new string("\t");
    out->append(l->ins);
    out->append(" ");
    vector<op_pair *>::iterator x = l->operands.begin();
    int c = 0;
    for (; x != l->operands.end(); x++)
    {
        if ((*x)->load_size != none)
        {
            out->append(size_to_string((*x)->load_size));
            out->append(" ");
        }
        (*x)->operand->accept(this);
        out->append(this->tmp->c_str());
        // out->append((*x)->operand->to_string());
        if (c != l->operands.size() - 1)
        {
            out->append(",");
        }
        c++;
    }
    this->tmp = out;
}
} // namespace ASSEMBLY
} // namespace LEAF
