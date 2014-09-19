#pragma once
#include "base/Buffer.h"

class ProtoBufHelper
{
public:    
    int Pack(const char* pszMetaName,const Buffer & objData,Buffer & buffer);
    int UnPack(const char* pszMetaName,const Buffer & buffer,Buffer objData);    
    //todo list meta sub attr
    //get descripter and sthm
};






