/*
 * =====================================================================================
 *
 *       Filename:  Noncopyable.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2014年05月22日 07时55分55秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *   Last Changed: 05/22/2014 07时59分04秒
 *
 * =====================================================================================
 */
#pragma once

//template<class U>
class NonCopyable
{
private:
	NonCopyable & operator = (NonCopyable & );
};
