#pragma once
#include "stdinc.h"

class StringUtil
{
public:
	static int Split(const char *pszStr, char *pszBuf, int iBufArrLen, int iBufStrLen, const char *pszSeparator);
	static int SplitNum(const char *pszStr, int *piBuf, int iBufArrLen, const char *pszSeparator);
    static void ReplaceAll(string & src , const string & match, const string &  repl);
};

