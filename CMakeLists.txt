cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_BUILD_TYPE Release)

SET(CMAKE_CXX_FLAGS "-Wall -Ofast -ffast-math -mtune=native")

########################################################################################################################

 project(CordicFunctionGen)

# #
 set(SOURCE_FILES
         ./src/CordicFunctionGen.cpp)

 add_executable(CordicFunctionGen ${SOURCE_FILES})

 ########################################################################################################################

 ########################################################################################################################

 project(EvalCordicFunction)

# #
 set(SOURCE_FILES
         ./src/cordic-test.cpp)

 add_executable(EvalCordicFunction ${SOURCE_FILES})

 ########################################################################################################################
