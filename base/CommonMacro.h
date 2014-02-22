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


SAFE_DELETE(p)	do{\
if((p)!= NULL)\
{\
	delete (p);\
	(p) = NULL;\
}}while(0)
