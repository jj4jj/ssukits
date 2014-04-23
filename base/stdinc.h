/*
 * =====================================================================================
 *
 *       Filename:  stdinclude.h
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

//--------------------std c ------------------------------


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <math.h>
#include <time.h>
#include <strings.h>
#include <stdint.h>
#include <errno.h>
#include <fcntl.h>
#include <stddef.h>
#include <setjmp.h>
#include <signal.h>
#include <assert.h>
//-------------------std cpp--------------------------


#include <string>
using std::string;
#include <vector>
using std::vector;
#include <map>
#include <set>
#include <algorithm>
//#include <multimap>
//#include <multiset>
#include <list>
#include <stack>
#include <iterator>
#include <bitset>
#include <stack>

#include <tr1/memory>
using std::tr1::shared_ptr;
#include <tr1/unordered_map>
#include <tr1/unordered_set>
using std::tr1::unordered_map;
using std::tr1::unordered_set;
#include <functional>

#include <iostream>


//#include <tr1/functional>
//using namespace std::tr1::placeholders;
//using std::tr1::bind;

//------------------gnu_cxx cpp---------------------


//------------------prototypes---------------------

//------------------linux system call -------------
#include <unistd.h>
#include <dirent.h>

#include <poll.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#include <arpa/inet.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>
#include <linux/un.h>
#include <sys/mman.h>
//thread
#include <pthread.h>


