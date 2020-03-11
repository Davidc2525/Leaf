

#ifndef _GRAPH_
#define _GRAPH_

#include "vector"
#include <iostream>
#include <string.h>
using namespace std;
namespace LEAF
{
namespace HELPER
{
namespace GRAPH
{

class Node;
class Edge;
class Graph;

class Node
{
public:
    Node(int type, string *name);
    string *name;
    int type;
};
class Edge
{
public:
    Edge(Node *a, Node *b, int rel);
    Node *nodea;
    Node *nodeb;
    int rel;
};

class Graph
{

public:
    Graph()
    {
        edges = new vector<Edge *>;
    };
    vector<Edge *> *edges;
    void add(Edge *val);
    vector<Edge *> *path_from(Node *a);
    void path_from(Node *a, vector<Edge *> *);
    //vector<Edge *> *filter_path_from_by_nodea(Node*);
    vector<Edge *> *filter_path_from_by_nodea(Node*,vector<Edge*>*list);
};

} // namespace GRAPH
} // namespace HELPER
} // namespace LEAF

#endif