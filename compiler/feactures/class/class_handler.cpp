

#include "class_handler.h"

namespace LEAF
{
namespace FEACTURE
{
namespace CLASS
{

void ClassHandler::draw(vector<Edge *> *list)
{
    string st("");
    vector<Edge *>::iterator x = list->begin();
    for (; x != list->end(); x++)
    {
        st.append(" ");
        st.append((*x)->nodea->name->c_str());
        if ((*x)->rel == IMPLEMENT)
        {
            st.append(" --> ");
        }
        else
        {
            st.append(" > ");
        }
        st.append((*x)->nodeb->name->c_str());
        st.append(",");
    }
    cout << st << endl;
};
void ClassHandler::implement(Node *a, Node *b)
{
    if (a->type == CLASS && b->type == INTERFACE)
    {
    }
    else
    {
        if (b->type == CLASS)
        {
            cout << "no puede implementar una clase: " << *a->name << " implement " << *b->name << "(class)" << endl;
        }
        cout << "no puede implementar: " << *a->name << " -> " << *b->name << endl;
        return;
    }
    if (this->check_implement(a, b))
    {
        this->add(new Edge(a, b, IMPLEMENT));
    }
};
void ClassHandler::extend(Node *a, Node *b)
{
    if (a->type == CLASS && b->type == CLASS)
    {
    }
    else
    {
        if (b->type == CLASS)
        {
            cout << "no puede heredar: " << *a->name << " -> " << *b->name << "(class)" << endl;
        }
        cout << "no puede heredar: " << *a->name << " -> " << *b->name << endl;
        return;
    }
    if (this->check_hierarchy(a, b))
    {
        this->add(new Edge(a, b, HIERARCHY));
    }
};

bool ClassHandler::check_implement(Node *a, Node *b)
{

    if (0 == strcmp(a->name->c_str(), b->name->c_str()))
    {
        throw "no se puede implementar asi misma";
    }

    if (this->cyclic(b, a))
    {
        throw "cyclic error (I)";
    }
    return true;
};
bool ClassHandler::check_hierarchy(Node *a, Node *b)
{

    if (0 == strcmp(a->name->c_str(), b->name->c_str()))
    {
        throw "no se puede heredar asi misma";
    }

    if (this->cyclic(b, a))
    {
        throw "cyclic error";
    }
    return true;
};

bool ClassHandler::cyclic(Node *a, Node *b)
{

    bool p = false;
    vector<Edge *>::iterator x = edges->begin();
    for (; x != edges->end(); x++)
    {
        if ((0 == strcmp((*x)->nodea->name->c_str(), a->name->c_str())) && (0 == strcmp((*x)->nodeb->name->c_str(), b->name->c_str())))
        {
            p = true;
            cout << "cyclic" << endl;
        }
        if (0 == strcmp((*x)->nodea->name->c_str(), b->name->c_str()) && 0 == strcmp((*x)->nodeb->name->c_str(), a->name->c_str()))
        {
            p = true;
            cout << *(*x)->nodeb->name << " no puede hererdar " << *(*x)->nodea->name << ", ya se hereda una a la otra" << endl;
        }
    }

    if (!p)
    {
        vector<Edge *> *result;
        vector<Edge *> *list = new vector<Edge *>;
        this->path_from(a, list);
        vector<Edge *> *path_from_na = this->filter_path_from_by_nodea(a, list);

        vector<Edge *>::iterator pfa_i = path_from_na->begin();

        for (; pfa_i != path_from_na->end(); pfa_i++)
        {
            result = new vector<Edge *>;
            Edge *edge = (*pfa_i);
            if (p)
                return p;

            this->path_from(edge->nodeb, result);
            vector<Edge *>::iterator result_i = result->begin();

            for (; result_i != result->end(); result_i++)
            {
                if (0 == strcmp((*result_i)->nodeb->name->c_str(), b->name->c_str()))
                {
                    p = true;
                }
            }
        }
    }

    cout << "hay camino de " << *a->name << " a " << *b->name << " ? " << (p ? "si" : "no") << endl;

    return p;
};

} // namespace CLASS
} // namespace FEACTURE
} // namespace LEAF
