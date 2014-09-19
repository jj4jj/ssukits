#the compiling files
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} ssukits_ipc_SRCS)

#base dir depend on itself and some other lib
include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}
	${PROJECT_SOURCE_DIR}
)


#it will generate a static lib for compiling faster
add_library(ssukits_ipc STATIC
  ${ssukits_ipc_SRCS}
)







