#the compiling files
file(GLOB_RECURSE ssukits_algo_SRCS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.cpp *.c *.h *.hpp *.cc)
# set language

#base dir depend on itself and some other lib
include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}
)

#it will generate a static lib for comiling faster
add_library(ssukits_algo STATIC
  ${ssukits_algo_SRCS}
)

#set_target_properties(ssukits_base PROPERTIES LINKER_LANGUAGE CXX)

