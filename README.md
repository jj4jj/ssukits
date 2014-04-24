##ssukits is a simple (game) server tookits package.
* Creating this rep just for studying and for fun . 
* Now , ssukits  only running in Linux evironment , and the implementing of it's function is just for studying and for fun .
	anyone can use it and distribute it , whatever you did with it .
* It's a study job , so there are many bugs in it . . . o__o


##Warnning
* Maybe u can't compile it succesfully , don't be panic or angry  ! since I commit code at any time ...
* I don't recommend you using it in your commercial project now , for many bugs in it !!  O__O (I'm decreasing them)

##What functions every directory included

* 1.base
	* it's the basic function of ssukits . with supporting Log ,File heper,
common time handling , string handling, and some utililized Class tools.
	* as so far , the implement of 'base' can't cross platform . in future,
it will satisfied the furture as a practical requiration .
* 2.net
	* It support a simple wrapper for tcp/udp/unix socket , and implement a 
reactor model tcp server . 
	* it's simple and stupid .
* 3.ipc
	* Wrapping the linux share memory and mmap , pipe . order to acquir a 
simple interface when using them .	
	* in future , maybe a RPC framework also be here.

* 4.thread
	* Wrapping the pthread functions , with every thread sync method , take
a class include the thread syncing object and operations set.	
	* Also , it should envole the user-thread model , coroutine . but it's
birth will be month later.

* 5.script
	* By implementing the LuaAgent that is a lua agent in c/cpp program so that using Lua script embed in c/cpp program easily .	
	* LuaBridge is a small and pretty lib for lua cpp binding . I choosed it .
* 6.algorithm
	* Just collecting common data struct and algorithm , when using it or debuging , I can know the every thing state in memory . so it's simple and supid but no many template argument like stl.
	* As so for, It include the AStar searching method , Red-black tree and so on ...

* 7.math
	* soem numeric computing should be easier without outter lib . just for 
studying and for fun .	

* 8.utility
	* some misc tools set . like Daemon implementing ,debug info ,Debuger,
crt , excutable file format and regexp , etc.  distance 
	* if you want to compile it to see the result ,
you need a linux eviroment , and cmake program , at least 2.6 version.
* 9.ai & machine learnning
	* some algorithm in ai and ml , just for my study ...

##Depends and testing
```
* 1.use cmake generate makefiles compiling .
	* the buiding dir must not be in the source dir.
* 2.make install
	* you modify the install dir in SRC/CMakeLists.txt 
* 3.run the testing ..
	* ...
```

