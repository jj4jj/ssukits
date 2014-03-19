#the compiling files
file(GLOB_RECURSE ssukits_script_SRCS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.cpp *.h)

#base dir depend on itself and some other lib
include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}
	${PROJECT_SOURCE_DIR}
	${LUA_INC}
	${LUA_BRIDGE_INC}
)


#it will generate a static lib for compiling faster
add_library(ssukits_script STATIC
  ${ssukits_script_SRCS}
)


#set_target_properties(ssukits_net PROPERTIES LINKER_LANGUAGE CXX)





