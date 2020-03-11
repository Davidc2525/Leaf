


#ifndef __I_ELEMENT_ACCEPTER__
#define __I_ELEMENT_ACCEPTER__

class ast_visitor;
#include "./ast_visitor.h"

/**
 * Interface que contiene los metodos aceptadores que se implementaran 
 * en los nodos del arbol (AST)
 * 
 * Los metodos siguen el diseño de nombres
 * prefix:sifux
 * donde el prefijo: accept
 * y e sifujo: 1,2,3,4,5,6...
 * 
 * de ese modo se mantiene un diseño de proposito general
 * 
 * Para la generacion de codigo se usan para llamar a el metodo visit#N, correspondiente
 * 
 * accept: usado para cargar un operando: load
 * accept1: usado para cargar un operando a un registro: load_reg
 * accept2: usado para aplicarle una operacion a un oeperando: operation.
 * 
 * se pueden añadir mas si es necesario, los demas se manteien sin un uso aun.
*/
class accepter
{

public:
    /**
     * En generacion de codigo es usado para invocar el visit -> load
    */
    virtual void accept(ast_visitor *v) = 0;
    /**
     * En generacion de codigo es usado para invocar el visit1 -> load_reg
    */
    virtual void accept1(ast_visitor *v) = 0;
    /**
     * En generacion de codigo es usado para invocar el visit2 -> operation
    */
    virtual void accept2(ast_visitor *v) = 0;
    /**
     * sin uso
    */
    virtual void accept3(ast_visitor *v) = 0;
    /**
     * sin uso
    */
    virtual void accept4(ast_visitor *v) = 0;
};

#endif