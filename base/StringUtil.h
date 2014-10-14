#pragma once
#include "stdinc.h"
#include "Buffer.h"

class StringUtil
{
public:
	static int Split(const char *pszStr, char *pszBuf, int iBufArrLen, int iBufStrLen, const char *pszSeparator);
	static int SplitNum(const char *pszStr, int *piBuf, int iBufArrLen, const char *pszSeparator);
    static void ReplaceAll(string & src , const string & match, const string &  repl);
    static size_t Hash(const char *  str);
    static size_t Hash(const string & s);
    static vector<string> SplitString(string & str,const char* pszSep);


public:
    static int  ToBase64(string & to,const Buffer & from);
    static int  FromBase64(Buffer & to,string & from);
};

