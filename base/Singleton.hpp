/*
 * =====================================================================================
 *
 *       Filename:  Singleton.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2014年05月22日 07时49分47秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *   Last Changed: 05/22/2014 07时59分46秒
 *
 * =====================================================================================
 */

#pragma once
#include "NonCopyable.hpp"

//single thread edition 
//multi thread edition
//
//
//
//
//

// just have single thread edition.
template<class HostType>
class Singleton : public NonCopyable
{
public:
	HostType & Instance()
	{
		static HostType s_Inst;
		return s_Inst;
	}
private:
	Singleton(){}
};



