
#the compiling files
file(GLOB_RECURSE ssukits_net_SRCS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.cpp *.h)


#base dir depend on itself and some other lib
include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}
)

#base dir linked lib dir
#
#link_directories(
#  /usr/local/lib
#)

#it will generate a static lib for comiling faster
add_library(ssukits_net STATIC
  ${ssukits_net_SRCS}
)
#set_target_properties(ssukits_net PROPERTIES LINKER_LANGUAGE CXX)




