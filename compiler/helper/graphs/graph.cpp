

#include "graph.h"
using namespace LEAF::HELPER::GRAPH;

Node::Node(int type, string *name)
{
    this->type = type;
    this->name = name;
};
Edge::Edge(Node *a, Node *b, int rel)
{
    nodea = a;
    nodeb = b;
    this->rel = rel;
};

vector<Edge *> *Graph::filter_path_from_by_nodea(Node *n, vector<Edge *> *list)
{
    vector<Edge *> *rf = new vector<Edge *>;
    vector<Edge *>::iterator x = list->begin();
    for (; x != list->end(); x++)
    {

        if (0 == strcmp((*x)->nodea->name->c_str(), n->name->c_str()))
        {
            rf->push_back((*x));
        }
    }

    return rf;
}
void Graph::add(Edge *val)
{
    edges->push_back(val);
};

void Graph::path_from(Node *a, vector<Edge *> *r)
{

    vector<Edge *>::iterator x = edges->begin();
    for (; x != edges->end(); x++)
    {

        if (0 == strcmp((*x)->nodea->name->c_str(), a->name->c_str()))
        {

            r->push_back((*x));
            path_from((*x)->nodeb, r);
        }
    }
};

vector<Edge *> *Graph::path_from(Node *a)
{
    vector<Edge *> *r = new vector<Edge *>;

    path_from(a, r);
    return r;
};
