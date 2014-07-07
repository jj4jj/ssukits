#pragma once
#include "base/CommonMacro.h"


class EncrptionHelper
{
public:
	// 0 is ok
    static int MD5(IN const Buffer & text,INOUT Buffer & md5sum);    
	static int RSAEncrypt(IN const Buffer & decrypt,INOUT Buffer & crypt,RSAParam * param); 
	static int RSADecrypt(IN const Buffer & crypt,INOUT Buffer & deypt,RSAParam * param); 
};










