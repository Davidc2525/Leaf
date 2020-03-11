

#ifndef _H_CLASS_HANDLER_
#define _H_CLASS_HANDLER_
#include "../../symb_tab/symbol_table.hpp"
#include "../../helper/graphs/graph.h"
using namespace LEAF::HELPER::GRAPH;
namespace LEAF
{
namespace FEACTURE
{
namespace CLASS
{
//type hierarchy
const int IMPLEMENT = 1;
const int HIERARCHY = 2;
//type node
const int INTERFACE = 1;
const int CLASS = 2;

class Entry : public Node
{
};
class Class : public Entry
{
};
class Interfaz : public Entry
{
};

/**
 * @brief manejador de clase, lleva contro y datos de herencia e implementaciones
 * de las clases, control de polimorfismo.
 * 
*/
class ClassHandler : public Graph
{
public:
    void implement(Node *a, Node *b);
    void extend(Node *a, Node *b);
    bool check_implement(Node *a, Node *b);
    bool check_hierarchy(Node *a, Node *b);
    bool cyclic(Node *a, Node *b);
    void draw(vector<Edge *> *list);

    vector<Interfaz*>*implements(Node*);
    vector<Class*>*extends(Node*);
};

} // namespace CLASS
} // namespace FEACTURE
} // namespace LEAF

#endif
