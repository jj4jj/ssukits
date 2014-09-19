#the compiling files
#file(GLOB_RECURSE ssukits_net_SRCS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.cpp *.h)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} ssukits_net_SRCS)


#base dir depend on itself and some other lib
include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}
)

#it will generate a static lib for comiling faster
add_library(ssukits_net STATIC
  ${ssukits_net_SRCS}
)



