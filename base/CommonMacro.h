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
if((p)!= NULL)\
{\
	delete (p);\
	(p) = NULL;\
}}while(0)


#ifndef SAFE_FREE
#define SAFE_FREE(p)    do{\
if((p)!= NULL)\
{\
	free((p));\
	(p) = NULL;\
}}while(0)


#ifndef MAX
#define MAX(a,b)	((a)>(b)?(a):(b))
#endif

#ifndef MIN
#define MIN(a,b)	((a)<(b)?(a):(b))
#endif


#ifndef
#define SWAP(a,b)	do{(a)=(a)+(b);(b)=(a)-(b);(a)=(a)-(b);}while(false)
#endif





