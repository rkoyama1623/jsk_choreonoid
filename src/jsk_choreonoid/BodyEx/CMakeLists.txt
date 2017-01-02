# cmake_minimum_required(VERSION 2.6)

# this CMakeList.txt defines following variables
# - LIBS_OUTPUT_DIR: directory for .so file
# - choreonoid_PLUGINDIR: directory for choreonoid plugin
#
# this CMakeList.txt defines following functions
# - add_jsk_choreonoid_python_module(xxx xxx.cpp)
#
# this CMakeList.txt include boost.python (includeing boost.python.numpy), libpython

# set output directory for .so
if (CATKIN_DEVEL_PREFIX)
set(LIBS_OUTPUT_DIR ${PROJECT_SOURCE_DIR}/src/jsk_choreonoid/BodyEx)
message(LIBS_OUTPUT_DIR= ${LIBS_OUTPUT_DIR})
else()
set(LIBS_OUTPUT_DIR ${PROJECT_BINARY_DIR}/python2.7/dist-packages/cnoid_pybody)
message(LIBS_OUTPUT_DIR= ${LIBS_OUTPUT_DIR})
endif()

# set libraries for boost.python
cmake_policy(SET CMP0003 NEW)
find_package(PythonLibs REQUIRED)
set(Boost_PYTHON_LIBRARIES ${Boost_PYTHON_LIBRSRIES} boost_numpy boost_python)
include_directories(${PYTHON_INCLUDE_DIRS} ${Boost_PYTHON_INCLUDE_DIRS})
# set choreonoid_PLUGINDIR
execute_process( # [todo] we should use find_package function (maybe)
  COMMAND pkg-config --variable=plugindir choreonoid
  OUTPUT_VARIABLE choreonoid_PLUGINDIR
  OUTPUT_STRIP_TRAILING_WHITESPACE)
# for debug
message(Boost_PYTHON_LIBRARIES= ${Boost_PYTHON_LIBRARIES})
message(Boost_PYTHON_INCLUDE_DIRS= ${Boost_INCLUDE_DIRS})
message(PYTHON_INCLUDE_DIRS= ${PYTHON_INCLUDE_DIRS})
message(choreonoid_PLUGINDIR= ${choreonoid_PLUGINDIR})

# if(UNIX)
#   SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
#   add_definitions("-pthread")
# endif()

function(add_jsk_choreonoid_python_module)
  set(target ${ARGV0})
  string(REGEX REPLACE "^Py(.+)$" "\\1" module ${target})
  set(sources ${ARGV})
  list(REMOVE_AT sources 0)

  add_library(${target} SHARED ${sources})

  if(NOT WIN32)
    set_target_properties(${target}  PROPERTIES
      COMPILE_DEFINITIONS "BOOST_PYTHON_USE_GCC_SYMBOL_VISIBILITY" )
  else()
    set_target_properties(${target}  PROPERTIES SUFFIX .pyd)
  endif()

  set_target_properties(${target}  PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${LIBS_OUTPUT_DIR}
    LIBRARY_OUTPUT_DIRECTORY ${LIBS_OUTPUT_DIR}
    RUNTIME_OUTPUT_NAME ${module}
    LIBRARY_OUTPUT_NAME ${module}
    PREFIX "")
  install(TARGETS ${target}
    RUNTIME DESTINATION ${LIBS_OUTPUT_DIR} CONFIGURATIONS Release Debug RelWithDebInfo MinSizeRel
    LIBRARY DESTINATION ${LIBS_OUTPUT_DIR} CONFIGURATIONS Release Debug RelWithDebInfo MinSizeRel)
endfunction()

# subdirectories
add_subdirectory(src)