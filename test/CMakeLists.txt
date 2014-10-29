#the compiling files
file(GLOB_RECURSE ssukits_test_SRCS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.cpp *.c *.h)
SET_SOURCE_FILES_PROPERTIES(${ssukits_test_SRCS} PROPERTIES LANGUAGE CXX )


#base dir depend on itself and some other lib
include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}
  ${LUA_BRIDGE_INC}
  ${LUA_INC}
)

#base dir linked lib dir
#
link_directories(
	${PROJECT_SOURCE_DIR}/bin
	${LUA_LIB}
	/usr/lib
)

#it will generate a exe
ADD_EXECUTABLE(pingpang_server
	${CMAKE_CURRENT_SOURCE_DIR}/PingPangServer.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/PingPangHandler.cpp
)
ADD_EXECUTABLE(pingpang_client
	${CMAKE_CURRENT_SOURCE_DIR}/PingPangClient.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/PingPangHandler.cpp
)
ADD_EXECUTABLE(pp_udp_server
	${CMAKE_CURRENT_SOURCE_DIR}/UdpServer.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/PingPangHandler.cpp
)
ADD_EXECUTABLE(pp_udp_client
	${CMAKE_CURRENT_SOURCE_DIR}/UdpClient.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/PingPangHandler.cpp
)

ADD_EXECUTABLE(test_base
	${CMAKE_CURRENT_SOURCE_DIR}/TestBase.cpp
)

ADD_EXECUTABLE(test_lua
	${CMAKE_CURRENT_SOURCE_DIR}/LuaTest.cpp
)


#assign custrom link lib
target_link_libraries(pingpang_server
	libssukits_base.a
	libssukits_net.a	
)
target_link_libraries(pingpang_client
	libssukits_base.a
	libssukits_net.a	
)
target_link_libraries(pp_udp_client
	libssukits_base.a
	libssukits_net.a	
)
target_link_libraries(pp_udp_server
	libssukits_base.a
	libssukits_net.a	
)
target_link_libraries(test_base
	libssukits_base.a
	libssukits_utility.a
)

target_link_libraries(test_lua
	libssukits_script.a
	libssukits_base.a
	liblua.a
	rt
	dl
)



#installing
INSTALL(TARGETS  pingpang_server DESTINATION net/)
INSTALL(TARGETS  pingpang_client DESTINATION net/)
INSTALL(TARGETS  pp_udp_client DESTINATION net/)
INSTALL(TARGETS  pp_udp_server DESTINATION net/)
INSTALL(TARGETS  test_base DESTINATION base/)

INSTALL(TARGETS  test_lua DESTINATION script/)
INSTALL(FILES  ${CMAKE_CURRENT_SOURCE_DIR}/TestLua.lua DESTINATION script/)


	
