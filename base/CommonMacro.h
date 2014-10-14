/*
 * =====================================================================================
 *
 *       Filename:  CommonMacro.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2014年05月22日 05时38分38秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *   Last Changed: 05/22/2014 05时43分59秒
 *
 * =====================================================================================
 */


#pragma once


#ifndef SAFE_DELETE
#define SAFE_DELETE(p)	do{\
	delete (p);\
	(p) = NULL;\
}while(0)
#endif


#ifndef SAFE_FREE
#define SAFE_FREE(p)    do{\
if((p)!= NULL)\
{\
	free((p));\
	(p) = NULL;\
}}while(0)
#endif


#ifndef MAX
#define MAX(a,b)	((a)>(b)?(a):(b))
#endif

#ifndef MIN
#define MIN(a,b)	((a)<(b)?(a):(b))
#endif



//since before c99, snprintf is depend on implementation
//maybe it's not end with \0
#ifndef SNPRINTF
#define SNPRINTF(s,n,f,args...)		do{\
    if(n>0)\
    {\
        snprintf((s),(n),(f),##args);\
        (s)[n-1]='\0';\
    }\
}while(false)
#endif

#ifndef STRNCPY
#define STRNCPY(d,n,s)  do{\
    if(n>0)\
    {\
        strncpy((d),(s),n);\
        (d)[(n)-1] = '\0';\
    }\
}while(0)

#endif
/////////////////////////////////////////////////////////////////////////////////




//must be a array , but a pointer
#ifndef DIM_OF_ARRAY
#define DIM_OF_ARRAY(array)     (sizeof((array))/sizeof((array)[0]))
#endif








//////////////////////////////////////////////////////////////////////////////////

#define IN
#define OUT
#define INOUT

#define USE_SSUKITS     using namespace ssukits;
#define BEGIN_NS_SSUKITS    namespace ssukits{		//begin for ssukits
#define END_NS_SSUKITS      };						//end for ssukits






