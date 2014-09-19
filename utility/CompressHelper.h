#pragma once

#include "base/Buffer.h"
class CompressHelper
{
public:
	static int Compress(int iMethod , const Buffer & in, Buffer & out,int param);
	static int UnCompress(int iMethod , const Buffer & in, Buffer & out,int param);
};


