#ifndef __VISITOR__
#define __VISITOR__

#include "../../ast/ast.hpp"
//using namespace LEAF::AST;

/**
 * 
 * Contiene metodo para inciar con el recorrido (visitas) en el ASU "node"
 * contenido en el primer parametro
 * 
*/
class visitor
{
public:
	visitor() { ; };
	~visitor() { ; };
	virtual void init(LEAF::AST::ASTU node) = 0;
};

#endif