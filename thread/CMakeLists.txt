#the compiling files
#file(GLOB_RECURSE ssukits_thread_SRCS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.cpp *.h)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} ssukits_thread_SRCS)


#base dir depend on itself and some other lib
include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}
)

#it will generate a static lib for comiling faster
add_library(ssukits_thread STATIC
  ${ssukits_thread_SRCS}
)

