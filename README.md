ssukits is a simple server tookits package.
<BR>
	now , only running it in Linux evironment , and implementing of it's function is just for studying and for fun .
	anyone can use it and distribute it , whatever you did with it .
<BR>
	ssukits include sub directories and their functions described as follow :
<BR>
1.base
<BR>
	it's the basic function of ssukits . with supporting Log ,File heper,
common time handling , string handling, and some utililized Class tools.
	as so far , the implement of 'base' can't cross platform . in future,
it will satisfied the furture as a practical requiration .
2.net
<BR>
	It support a simple wrapper for tcp/udp/unix socket , and implement a 
reactor model tcp server . 
	it's simple and stupid .
3.ipc
<BR>
	Wrapping the linux share memory and mmap , pipe . order to acquir a 
simple interface when using them .	
	in future , a RPC framework also be here.


4.thread
<BR>
	Wrapping the pthread functions , with every thread sync method , take
a class include the thread syncing object and operations set.	
	Also , it should envole the user-thread model , coroutine . but it's
birth will be month later.

5.script
<BR>
	By implementing the LuaAgent that is a lua agent in c/cpp program so that using Lua script embed in c/cpp program easily .	
	
6.algorithm
<BR>
	Just collecting common data struct and algorithm , when using it or debuging , I can know the every thing state in memory . so it's simple and supid but no many template argument like stl.
	As so for, It include the AStar searching method , Red-black tree and so on ...

7.math
<BR>
	soem numeric computing should be easier without outter lib . just for 
studying and for fun .	

8.utility
<BR>
	some misc tools set . like Daemon implementing ,debug info ,Debuger,
crt , excutable file format and regexp , etc.  
<BR>



	if you want to compile it to see the result ,
you need a linux eviroment , and cmake program , at least 2.6 version.
<BR>
1.use cmake generate makefiles compiling .
<BR>
	the buiding dir must not be in the source dir.
2.make install
<BR>
	you modify the install dir in SRC/CMakeLists.txt 
3.run the testing ..
<BR>
	...


