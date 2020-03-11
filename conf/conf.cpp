


#include "./conf.h"

namespace LEAF
{
namespace CONF
{
//Conf &ConfManager::conf = *new Conf;
ConfManager::ConfManager(){};
Conf &ConfManager::get()
{
    static Conf conf;
    return conf;
}
} // namespace CONF

} // namespace LEAF
