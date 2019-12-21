/**
 * 
 * Para la generacion de codigo se usa un patron de diseño llamado (visitor)
 * aqui se espesifica la interfaz para el visitor, donde cada visitor en concreto 
 * sera por lo generar una implementacion que estara dirigida
 * a la generacion de codigo de diferentes plataformas y arquitecturas de procesador
 * 
 * plataformas y arquitecturas que se cubrirar
 * 
 * linux 64 y 32 bits
 * windows 64 y 32 bits
 * 
 * las carpetas de cada implementacion tendra su nombre de la siguiente manera
 * arquitectura + plataforma
 * eg:
 * x86_64_linux
 * x86_64_windows
 * 
 * para las implementaciones con caracteristicas adicionales se les añade la descripccion
 * de la misma al final de el nombre.
 * eg:
 * 	x86_64_linux_mint
 * 	x86_64_linux_ubuntu
 * 	x86_64_windows_xp
 * 	x86_64_windows_7
 * 	x86_64_windows_10
 *	arm_32_rashberry
 * 
 * 
 * Todas las implementaciones solo tienen que implementar la interface (ast_visitor)
 * para la generacion de codigo ya sea en samblador o directamente a codigo maquina u otro 
 * derivado intermedio
 * 
*/
#ifndef __AST_VISITOR__
#define __AST_VISITOR__

#include "visitor.h"
#include "ast_visitor_args.h"
//#include "../../tree.hpp"
//#include "../../ast/statement.hpp"
#include "../../ast/ast.hpp"

/**
 * Genera el codigo para los metodos de los diferentes tipos a sobrecargar
 * 
 * Cada tipo tiene sus metodos visits, donde se diferencia de los demas
 * por su tipo (NAME), creando la plantilla para la interface (ast_visitor)
*/
#define VISITS(NAME)                 \
	virtual void visit(NAME *) = 0;  \
	virtual void visit1(NAME *) = 0; \
	virtual void visit2(NAME *) = 0; \
	virtual void visit3(NAME *) = 0; \
	virtual void visit4(NAME *) = 0;

using namespace LEAF::AST;

/**
 * 
 * @brief Interface para recorer (visitar) el arbol sintactico.
 * 
 * Se usa para recorer (visitar) y generar principalmente el codigo 
 * para poder compilar el codigo fuente.
 * 
 * Puede ser usado para generar diferentes representaciones
 * del AST
 * 
 * Todos sus metodos son puros.
 * 
 * Todos los metodos tienen el prefijo: visit, incrementando en uno el sufijo: visit,visit1 ... visit3
 * de esa manera se mantiene una interface de diseño general, lo cual puede ser usado para otros fines
 * Representacion para generacion de codigo: 
 *
 * Metodo:Representacion 
 * -----------
 * 		visit: load
 * 		visit1: load_reg
 * 		visit2: operation	
 * 	...
 * ------------
*/
class ast_visitor : public visitor
{
public:
	//ast_visitor(ast_visitor_args *args){}
	//~ast_visitor(){}
	VISITS(BlockStatemntNode)
	VISITS(FuncDeclStatementNode)
	VISITS(BinOp)
	VISITS(Ident)
	VISITS(VarDecl)
	VISITS(Member)
	VISITS(Empty)
	VISITS(AssignStatement)
};

#endif