


#include "register_manager.hpp"

RegisterManager::RegisterManager()
{

    providers = (RegisterProvider **)malloc(sizeof(RegisterProvider) * 5);
}

RegisterManager *RegisterManager::getInstance()
{

    static RegisterManager inst;
    return &inst;
}

int RegisterManager::addProvider(RegisterProvider *provider)
{
    int current = this->ids;
    this->ids++;

    providers[current] = provider;

    return current;
}

RegisterProvider *RegisterManager::getProvider(int provider_descriptor)
{
    if (provider_descriptor > ids || provider_descriptor < 0)
    {
        throw "Proveedor no existe";
    }
    return providers[provider_descriptor];
}

RegisterProvider *RegisterManager::getProviderBy(int arc)
{
    RegisterProvider *provider = NULL;

    for (int p = 0; p < ids; p++)
    {
        if(arc==providers[p]->getArc()){
            provider = providers[p];
            return provider;
        }
    }
    if(provider == NULL){
        throw "Proveedor no existe";
    }

    return provider;
}