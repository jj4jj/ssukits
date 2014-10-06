#include "base/Log.h"
#include "ProtoBufHelper.h"

using namespace ::google::protobuf;

int     ProtoBufHelper::Init(const char* pszMetaFileName)
{
    //loading meta
    const DescriptorPool * root =  DescriptorPool::generated_pool();
    assert(root);
    const FileDescriptor * fileDescriptor = root->FindFileByName(string(pszMetaFileName));
    if(!fileDescriptor)
    {
        LOG_FATAL("file descriptor = %s not found !",pszMetaFileName);
        return -1;
    }
    m_pDescriptorPool = const_cast<DescriptorPool*>(fileDescriptor->pool());
    assert(m_pDescriptorPool);
    m_pObjFactory = MessageFactory::generated_factory();
    assert(m_pObjFactory);
    //////////////////////////////////////////////////////////////
    
    return 0;
}
int     ProtoBufHelper::GetPackSize(void* obj)
{
    Message*    msg  =  (Message*)obj;//m_pObjFactory->GetPrototype(meta);
    return    msg->ByteSize();
}
int     ProtoBufHelper::Pack(void* obj,Buffer & buffer)
{
    Message*    msg  =  (Message*)obj;//m_pObjFactory->GetPrototype(meta);
    //const Descriptor* meta =  msg->GetDescriptor();
    bool ret =  msg->SerializeToArray(buffer.pBuffer, buffer.iCap);
    if(!ret)
    {
        LOG_ERROR("meta name = %s pack error !",msg->GetDescriptor()->name().c_str());
        buffer.iUsed = 0;
        return -1;
    }
    buffer.iUsed = msg->ByteSize();
    return 0;
}
int     ProtoBufHelper::UnPack(const char* pszMetaName,Buffer & buffer,void * * ppObj)
{
    *ppObj = NULL;
    const Descriptor  *   meta = m_pDescriptorPool->FindMessageTypeByName(string(pszMetaName));
    if(!meta)
    {
        LOG_ERROR("meta name = %s not found !",pszMetaName);
        return -1;
    }
    const Message * prototype = m_pObjFactory->GetPrototype(meta);
    if(!prototype)
    {
        LOG_ERROR("meta name = %s proto not found !",pszMetaName);
        return -1;
    }
    Message * msg = prototype->New();
    if(!msg)
    {
        LOG_ERROR("meta name = %s new fail !",pszMetaName);
        return -1;
    }
    if(!msg->ParseFromArray(buffer.pBuffer,buffer.iUsed))
    {
        LOG_ERROR("meta name = %s unpack error ! buffer sz = %d",pszMetaName,buffer.iUsed);
        
        
        return -1;
    }    
    *ppObj = msg;
    return 0;
}
void    ProtoBufHelper::Visual(void* pObj,string & s)
{
    Message*  msg = (Message*)pObj;
    s = msg->DebugString();
}


