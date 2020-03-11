


#ifndef __AST_VISITOR_ARGS__
#define __AST_VISITOR_ARGS__

#include <string>
#include "../../assembly/assembly"

using namespace std;

/**
 * Argumentos para el visitor usado
*/
class ast_visitor_args
{
public:
	/**
	 * Nombre del archivo de salida
	 * ejemplo:
	 * 		programa.asm
	 * 		programa.s
	*/
	string *outfile;
	Assembly *ass;
};

#endif