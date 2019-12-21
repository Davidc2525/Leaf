#include "register_provider.hpp"

/**
 * Encontrar el registro menos usado
 * 
*/
Register *RegisterProvider::lessBussy()
{
    Register *less = NULL;
    int gtr = 1000000;
    for (auto x = registers_.begin(); x != registers_.end(); x++)
    {
        if ((*reservers)[(*x)->id]->size() < gtr && (*x)->bits == getArc())
        {
            gtr = (*reservers)[(*x)->id]->size();
            less = get((*x)->id);
        }
    }
    if (less == NULL)
    {
        throw "No se pudo encotrar el registro menos usado.";
    }
    return less;
}

bool RegisterProvider::isBussy(int id)
{
    //cout<<"isBussy reg: "<<id<<((*reservers)[id]->size()>0)<<endl;
    return (*reservers)[id]->size() > 0;
};

Register *RegisterProvider::get(int id)
{
    Register *found = NULL;
    vector<Register *>::iterator it = registers_.begin();
    for (std::vector<Register *>::iterator x = it; x != registers_.end(); x++)
    {
        if (((Register *)*x)->id == id)
        {
            int dis = distance(it, x);
            found = registers_[dis];
            //cout << ";index: " << dis << " reg: " << registers_[dis]->getName() << endl;
        }
    }
    return found;
};

Register *RegisterProvider::get_by_name(const char *name)
{
    return get_by_name2((char *)name);
};

Register *RegisterProvider::get_by_name3(string *name)
{
    return get_by_name(name->c_str());
};

Register *RegisterProvider::get_by_name2(char *name)
{
    Register *found = NULL;
    vector<Register *>::iterator it = registers_.begin();
    for (std::vector<Register *>::iterator x = it; x != registers_.end(); x++)
    {
        if (0 == strcmp(name, ((Register *)*x)->getName()))
        {
            int dis = distance(it, x);
            found = registers_[dis];
        }
    }
    return found;
};

void RegisterProvider::free(int id)
{
    (*reservers)[id]->pop();
};
void RegisterProvider::free_by_name(const char *name)
{
    free_by_name2((char *)name);
};
void RegisterProvider::free_by_name2(char *name)
{
    Register *found = NULL;
    vector<Register *>::iterator it = registers_.begin();
    for (std::vector<Register *>::iterator x = it; x != registers_.end(); x++)
    {
        if (0 == strcmp(name, ((Register *)*x)->getName()))
        {
            (*reservers)[(*x)->id]->pop();
            break;
            //cout << ";index: " << dis << " reg: " << registers_[dis]->getName() << endl;
        }
    }
};
void RegisterProvider::free_by_name3(string *name)
{
    free_by_name(name->c_str());
};

Register *RegisterProvider::reserve()
{
    Register *found = NULL;
    vector<Register *>::iterator it = registers_.begin();
    for (std::vector<Register *>::iterator x = it; x != registers_.end(); x++)
    {
        //cout<<"bussi: "<<isBussy((*x)->id)<<" reg: "<<(*x)->getName()<<" arc: "<< getArc()<<endl;
        if (!isBussy((*x)->id) && (*x)->bits == getArc() && (*x)->reservable)
        {
            int dis = distance(it, x);
            found = registers_[dis];
            break;
            //cout << ";index: " << dis << " reg: " << registers_[dis]->getName() << endl;
        }
    }
    if (found == NULL)
    {
        found = lessBussy();
    }
    if (found == NULL)
    {
        throw "No hay Registro disponible";
    }

    (*reservers)[found->id]->push(1);
    //cout<<"top: "<<(*reservers)[found->id]->size()<<",bussy: "<<((*reservers)[found->id]->size()>0)<<",csm : "<<isBussy(found->id)<<endl;
    return found;
};
Register *RegisterProvider::reserve_by_id(int id)
{
    Register *found = NULL;
    vector<Register *>::iterator it = registers_.begin();
    for (std::vector<Register *>::iterator x = it; x != registers_.end(); x++)
    {
        //cout<<"bussi: "<<isBussy((*x)->id)<<" reg: "<<(*x)->getName()<<" arc: "<< getArc()<<endl;
        if (id == (*x)->id && (*x)->reservable)
        {
            int dis = distance(it, x);
            found = registers_[dis];
            break;
            //cout << ";index: " << dis << " reg: " << registers_[dis]->getName() << endl;
        }
    }

    if (found == NULL)
    {
        throw "No hay Registro disponible";
    }

    (*reservers)[found->id]->push(1);
    //cout<<"top: "<<(*reservers)[found->id]->size()<<",bussy: "<<((*reservers)[found->id]->size()>0)<<",csm : "<<isBussy(found->id)<<endl;
    return found;
};
Register *RegisterProvider::reserve_by_name(const char *name)
{
    return reserve_by_name2((char *)name);
};
Register *RegisterProvider::reserve_by_name2(char *name)
{
    Register *found = NULL;
    vector<Register *>::iterator it = registers_.begin();
    for (std::vector<Register *>::iterator x = it; x != registers_.end(); x++)
    {
        //cout<<"bussi: "<<isBussy((*x)->id)<<" reg: "<<(*x)->getName()<<" arc: "<< getArc()<<endl;
        if (0 == strcmp(name, ((Register *)*x)->getName()) && (*x)->reservable)
        {
            int dis = distance(it, x);
            found = registers_[dis];
            break;
            //cout << ";index: " << dis << " reg: " << registers_[dis]->getName() << endl;
        }
    }

    if (found == NULL)
    {
        throw "No hay Registro disponible";
    }

    (*reservers)[found->id]->push(1);
    //cout<<"top: "<<(*reservers)[found->id]->size()<<",bussy: "<<((*reservers)[found->id]->size()>0)<<",csm : "<<isBussy(found->id)<<endl;
    return found;
};
Register *RegisterProvider::reserve_by_name3(string *name)
{
    return reserve_by_name(name->c_str());
};

Register *RegisterProvider::reserve_by_size(int size)
{
    Register *found = NULL;
    vector<Register *>::iterator it = registers_.begin();
    for (std::vector<Register *>::iterator x = it; x != registers_.end(); x++)
    {
        //cout<<"bussi: "<<isBussy((*x)->id)<<" reg: "<<(*x)->getName()<<" arc: "<< getArc()<<endl;
        if (!isBussy((*x)->id)&&(*x)->reservable && (*x)->bits == size)
        {
            int dis = distance(it, x);
            found = registers_[dis];
            break;
            //cout << ";index: " << dis << " reg: " << registers_[dis]->getName() << endl;
        }
    }

    if (found == NULL)
    {
        throw "No hay Registro disponible";
    }

    (*reservers)[found->id]->push(1);
    return found;
};

Register *RegisterProvider::get_by_size(int size)
{
    Register *found = NULL;
    vector<Register *>::iterator it = registers_.begin();
    for (std::vector<Register *>::iterator x = it; x != registers_.end(); x++)
    {
        //cout<<"bussi: "<<isBussy((*x)->id)<<" reg: "<<(*x)->getName()<<" arc: "<< getArc()<<endl;
        if ( (*x)->bits == size)
        {
            int dis = distance(it, x);
            found = registers_[dis];
            break;
            //cout << ";index: " << dis << " reg: " << registers_[dis]->getName() << endl;
        }
    }

    if (found == NULL)
    {
        throw "No hay Registro disponible";
    }

    (*reservers)[found->id]->push(1);
    return found;
};

int RegisterProvider::getArc(){NOIMPLEMENT};