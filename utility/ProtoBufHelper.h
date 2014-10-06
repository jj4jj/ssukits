#pragma once
#include "base/stdinc.h"
#include "base/Buffer.h"

#include "google/protobuf/compiler/code_generator.h"
#include "google/protobuf/compiler/cpp/cpp_generator.h"
#include "google/protobuf/compiler/python/python_generator.h"

//#include "google/protobuf/compiler/plugin.pb.h"
#include "google/protobuf/compiler/plugin.h"

#include "google/protobuf/descriptor.h"
#include "google/protobuf/message.h"
#include "google/protobuf/compiler/parser.h"
#include "google/protobuf/compiler/importer.h"


class ProtoBufHelper
{
public:
    int     Init(const char* pszMetaFileName);
    int     GetPackSize(void* obj);
    int     Pack(void* obj,Buffer & buffer);
    int     UnPack(const char* pszMetaName,Buffer & buffer,void * * ppObj);
    void    Visual(void* pObj,string & s);

public:
    template<class T>
    void    FreeObj(T* pObj){delete pObj;}

private:
    google::protobuf::DescriptorPool*       m_pDescriptorPool;
    google::protobuf::MessageFactory*       m_pObjFactory;
    
    
};





