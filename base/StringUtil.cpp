#include "StringUtil.h"


#if 1
int StringUtil::Split(const char *pszStr, char *pszBuf, int iBufArrLen, int iBufStrLen, const char *pszSeparator)
{
    int iCurr = 0;
	int iStrLen = 0;
	char *pszCurr = 0;

	pszCurr = strstr((char*)pszStr, pszSeparator);
	while(pszCurr != 0 && iCurr < iBufArrLen)
	{
		iStrLen = pszCurr - pszStr < iBufStrLen - 1 ? pszCurr - pszStr : iBufStrLen - 1;
		if(iStrLen > 0)
		{
			strncpy(pszBuf, pszStr, iBufStrLen);
			pszBuf[iStrLen] = '\0';
		}

		++iCurr;
		pszStr = pszCurr + strlen(pszSeparator);
		pszCurr = strstr((char*)pszStr, pszSeparator);
		pszBuf += iBufStrLen;
	}
	if(*pszStr != '\0' && iCurr < iBufArrLen)
	{
		iStrLen = strnlen(pszStr, iBufStrLen);

		if(iStrLen > 0)
		{
			strncpy(pszBuf, pszStr, iBufStrLen);
			pszBuf[iStrLen] = '\0';
			++iCurr;
		}
	}

	return iCurr;
}

int StringUtil::SplitNum(const char *pszStr, int *piBuf, int iBufArrLen, const char *pszSeparator)
{
    char pszTmpStr[20];
	int iCurr = 0;
	int iStrLen = 0;
	char *pszBuf = 0;
    
	pszBuf = strstr((char*)pszStr, pszSeparator);
	while(pszBuf != 0 && iCurr < iBufArrLen)
	{
		iStrLen = pszBuf - pszStr;
		if(iStrLen > 12)
		{
			return -1;
		}
		if(iStrLen > 0)
		{
			strncpy(pszTmpStr, pszStr, iStrLen);
			pszTmpStr[iStrLen] = '\0';
			piBuf[iCurr] = atoi(pszTmpStr);
		}

		++iCurr;
		pszStr = pszBuf + strlen(pszSeparator);
		pszBuf = strstr((char*)pszStr, pszSeparator);
	}
	if(*pszStr != '\0' && iCurr < iBufArrLen)
	{
		iStrLen = strnlen(pszStr, 20);
		if(iStrLen > 12)
		{
			return -1;
		}
		if(iStrLen > 0)
		{
			strncpy(pszTmpStr, pszStr, iStrLen);
			pszTmpStr[iStrLen] = '\0';
			piBuf[iCurr] = atoi(pszTmpStr);
			++iCurr;
		}
	}
	return iCurr;
}

void StringUtil::ReplaceAll(string & src , const string & match, const string &  repl)
{
    do
    {
        size_t pos = src.find(match);    
        if(pos == string::npos)
        {
            break;
        }
        src.replace(pos,match.length(),repl);
    }while(true);
}
size_t StringUtil::Hash(const string & s)
{
    return Hash(s.c_str());
}
//BKDRHash
size_t StringUtil::Hash(const char *  str)
{
    unsigned int seed = 13131; // 31 131 1313 13131 131313 etc..
    unsigned int hash = 0;
    while (*str)
    {
        hash = hash * seed + (*str++);
    }
    return (hash & 0x7FFFFFFF);
}
vector<string> StringUtil::SplitString(string & str,const char* pszSep)
{
    vector<string>  splits;
    size_t start = 0;
    size_t seplen = strlen(pszSep);
    size_t pos  = 0;
    do
    {
        pos = str.find(pszSep,start);        
        if(pos != string::npos && pos > start)
        {
            splits.push_back(str.substr(start,pos));
        }
        start = pos+seplen;
    }
    while(pos != string::npos);
    if(start < str.length() )
    {
        splits.push_back(str.substr(start));
    }    
    return splits;
}

#endif
