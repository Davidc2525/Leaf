
#ifndef _CONF_H_
#define _CONF_H_

#include <iostream>
#include <map>
#include <string.h>
using namespace std;

namespace LEAF
{
namespace CONF
{
struct str_cmp
{
    bool operator()(const char *v1, const char *v2) const
    {
        return strcmp(v1, v2) == 0;
    }
};
typedef void *conf_value;
typedef const char *conf_key;
class KeyNoFound
{
};

class Conf
{
private:
    map<conf_key, conf_value> items;
    bool start_with(const char *v1, const char *v2)
    {

        for (int x = 0; x < strlen(v1); x++)
        {
            if (v1[x] != v2[x])
                return false;
        }
        return true;
    }

    int to(const char *v1, const char *v2)
    {
        int index = 0;
        for (int x = 0; x < strlen(v1); x++)
        {
            if (v1[x] != v2[x])
                return index;
            index++;
        }
        return index;
    };

    const char *sub_from(const char *v1, const char *v2)
    {
        int index = to(v1, v2);
        string nc(v2);
        string nc2 = nc.substr(index);
        string *r = new string(nc2);
        return r->c_str();
    }

public:
    void set(conf_key k, conf_value v)
    {
        items.insert(make_pair(k, v));
    };

    conf_value get(conf_key k)
    {
        for (auto x = items.begin(); x != items.end(); x++)
        {
            if (strcmp(k, x->first) == 0)
            {
                return x->second;
            }
        }

        throw *new KeyNoFound;
        return NULL;
    }
    void set(conf_key k, conf_key v)
    {
        set(k, (conf_value)v);
    };

    template <class T>
    void set(conf_key k, T v)
    {
        set(k, (conf_value)v);
    }

    template <class T>
    T get(conf_key k)
    {
        return (T)get(k);
    }

    template <class T>
    T get(conf_key k, T def)
    {
        try
        {
            return (T)get(k);
        }
        catch (KeyNoFound ex)
        {
            cout << "WARN: key no found: '" << k << "' returnig default value." << endl;
            return def;
        }
    }

    conf_key get_str(conf_key k)
    {
        return (conf_key)items[k];
    };

    /**
     * @brief Crean una nueva instancia de configuracion local y agrupa sus configuraciones
     * ejemplo:
     *  leaf.compiler.asm.section.text.show = true
     *  leaf.compiler.asm.section.data.show = true
     * 
     * invokamos el metodo group pasandole el nombre del grupo (con el punto al final)
     * conf.group("leaf.compiler.asm.section.")
     * 
     * queda un nuevo objeto Conf con la siguiente configuracion:
     *  
     *  text.show = true
     *  data.show = true
     * 
     * quita el prefijo con el que se agrupo para una rapida y mas comoda recuperacion de la informacion
     * 
    */
    Conf &group(conf_key g)
    {
        Conf *new_group = new Conf;

        for (auto x = items.begin(); x != items.end(); x++)
        {
            if (start_with(g, (*x).first))
            {
                new_group->set(sub_from(g, (*x).first), (*x).second);
            }
        }
        return *new_group;
    }
};

class ConfManager
{
private:
    //static Conf &conf;
    ConfManager();

public:
    static Conf &get();
};

}; // namespace CONF

}; // namespace LEAF

#endif
