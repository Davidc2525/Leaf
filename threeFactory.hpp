
#include "tree.hpp"

class ThreeFactory{ 
    private: 
        ThreeFactory();
        int ids = 0;
        AstHandler** handlers = (AstHandler**)malloc(sizeof(AstHandler)*100);
        //static ThreeFactory * instance;
    public:
        static ThreeFactory * getInstance();
        AstHandler * getAstHandler(int id);
        int addAstHandler(AstHandler *);
};