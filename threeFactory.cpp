#include "threeFactory.hpp"

ThreeFactory::ThreeFactory() {}

ThreeFactory *ThreeFactory::getInstance()
{

    static ThreeFactory inst;
    return &inst;
}

AstHandler *ThreeFactory::getAstHandler(int id)
{
    if (id > ids)
    {
        // cout << "Manejador no registrado: "<<id;
        throw "Manejador no registrador";
        //exit(1);
    }

    AstHandler *found = handlers[id];
    if (!found)
    {
        //cout << "Manejador no registrado";
        throw "Manejador no registrador";
    }

    return found;
}

int ThreeFactory::addAstHandler(AstHandler *ast_h)
{
    int index = ids;
    handlers[index] = ast_h;
    ids++;
    return index;
}