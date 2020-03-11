


#ifndef _REGISTER_MANAGER_H_
#define _REGISTER_MANAGER_H_

#include "register_provider.hpp"


/**
 * Proveedor de registros por defecto
 * 
*/
class RegisterManager{ 
    private: 
        RegisterManager();
        int ids = 0;
        RegisterProvider** providers;
        //static RegisterManager * instance;
    public:
        static RegisterManager * getInstance();
        RegisterProvider * getProvider(int id);
        RegisterProvider * getProviderBy(int arc);
        int addProvider(RegisterProvider *);


};

#endif
