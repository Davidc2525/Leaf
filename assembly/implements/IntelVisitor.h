/**
 * Convierte los objetos en su representacion a cadena
 * para ser copiados en el archivo de salida del programa ensamblador
 * 
 * La sintaxis de salida es la espesificada por Intel, para ser 
 * ensamblado por NASM u otros.
 * 
 * @author Colmenares David [david25pcxtreme@gmail.com]
 * */

#ifndef _INTEL_VISITOR_
#define _INTEL_VISITOR_

#include "../assembly"
#include <vector>
#include <string>
#include <fstream>
using namespace std;
namespace LEAF
{
namespace ASSEMBLY
{

class IntelVisitor : public SlockVisitor
{
private:
    string *tmp;

public:
    IntelVisitor(){};
    string *get_src();
    void visit(Label *);
    void visit(MemoryOperand *);
    void visit(RegisterOperand *);
    void visit(InmediateIntOperand *);
    void visit(DInstruccion *);
};

} // namespace ASSEMBLY
}; // namespace LEAF

#endif