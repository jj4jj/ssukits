
#the compiling files
file(GLOB_RECURSE ssukits_utility_SRCS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.cpp *.c *.h *.hpp *.cc)

include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}
)

#it will generate a static lib for comiling faster
add_library(ssukits_utility STATIC
  ${ssukits_utility_SRCS}
)




