
# ssukits cmake cfg 
project(ssukits CXX)
set(ssukits_VERSION 0.0.1)

# 
cmake_minimum_required(VERSION 2.6)

# building must be in not source
string(COMPARE EQUAL "${CMAKE_SOURCE_DIR}" "${CMAKE_BINARY_DIR}" 
BUILDING_IN_SOURCE)

if(BUILDING_IN_SOURCE)
  message(FATAL_ERROR
    "ssukits compile dir must not be source dir , please remove 'CMakeCache.txt' in current dir , then create a building dir in which dir you can exec commands like this 'cmake <src dir> [options]' "
  )
endif()

# compile option
option(DEBUG "Debug mode" 1)
option(PCH "Use precompiled headers" 0)

if(DEBUG)
  message("Build in debug-mode   : Yes")
  set(CMAKE_BUILD_TYPE Debug)
else()
  set(CMAKE_BUILD_TYPE Release)
  message("Build in debug-mode   : No  (default)")
endif()



# debug common
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -Wall -Wfatal-errors -Wextra -g -rdynamic -gdwarf-2 -g3 -Wl,-Bdynamic")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -rdynamic -gdwarf-2 -Wall -Wfatal-errors -Wextra -Wno-unused-parameter")

# release mode
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} --no-warnings ")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} --no-warnings")


# output dir
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)




# install dir
set(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR}/../inst-ssukits)


add_subdirectory(base)
add_subdirectory(net)
#add_subdirectory(io)
#add_subdirectory(math)
#add_subdirectory(thread)
add_subdirectory(utility)
add_subdirectory(test)

