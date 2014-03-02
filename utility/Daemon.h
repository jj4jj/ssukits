#include "base/Singleton.hpp"
#include "base/Log.h"

class Daemon : Singleton<Daemon>
{
public:
    //enbale this process become a daemon process.
    //todo
    int    Create();
};






