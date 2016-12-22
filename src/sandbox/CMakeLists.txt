cmake_minimum_required(VERSION 2.6)
cmake_policy(SET CMP0003 NEW)
find_package(PythonLibs)
find_package(Boost COMPONENTS python)
include_directories(${Boost_INCLUDE_DIRS} ${PYTHON_INCLUDE_DIRS})

# target_link_libraries(hello_ext ${PYTHON_LIBRARIES} ${Boost_LIBRARIES})

include(FindPkgConfig)
find_package(choreonoid)
if(NOT ${choreonoid_FOUND})
	find_package(PkgConfig)
	pkg_check_modules(choreonoid choreonoid REQUIRED)
endif(NOT ${choreonoid_FOUND})
include_directories(${choreonoid_INCLUDE_DIRS})
message("choreonoid_CFLAGS: ${choreonoid_CFLAGS}")
message("choreonoid_LDFLAGS :${choreonoid_LDFLAGS}")
message("choreonoid_INCLUDE_DIRS: ${choreonoid_INCLUDE_DIRS}")
message("choreonoid_LIBRARY_DIRS: ${choreonoid_LIBRARY_DIRS}")
message("choreonoid_LIBRARIES: ${choreonoid_LIBRARIES}")

execute_process( # [todo] we should use find_package function (maybe)
  COMMAND pkg-config --variable=plugindir choreonoid
  OUTPUT_VARIABLE choreonoid_PLUGINDIR
  RESULT_VARIABLE RESULT
  OUTPUT_STRIP_TRAILING_WHITESPACE)
message("choreonoid_PLUGINDIR: ${choreonoid_PLUGINDIR}")

if(UNIX)
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
  add_definitions("-pthread")
endif()

function(add_cnoid_python_module)
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
    RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/${CNOID_PYTHON_SUBDIR}/cnoid
    LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/${CNOID_PYTHON_SUBDIR}/cnoid
    RUNTIME_OUTPUT_NAME ${module}
    LIBRARY_OUTPUT_NAME ${module}
    PREFIX "")

  install(TARGETS ${target}
    RUNTIME DESTINATION ${CNOID_PYTHON_SUBDIR}/cnoid CONFIGURATIONS Release Debug RelWithDebInfo MinSizeRel
    LIBRARY DESTINATION ${CNOID_PYTHON_SUBDIR}/cnoid CONFIGURATIONS Release Debug RelWithDebInfo MinSizeRel)

endfunction()

function(apply_common_setting_for_python_module target)
  set(headers ${ARGV1})
  set_target_properties(${target} PROPERTIES VERSION ${CNOID_VERSION})
  apply_common_setting_for_target(${target} "${headers}")

  if(INSTALL_SDK)
    if(MSVC)
      install(TARGETS ${target}
        ARCHIVE DESTINATION lib CONFIGURATIONS Release Debug RelWithDebInfo MinSizeRel)
    endif()
    if(headers)
      file(RELATIVE_PATH rel_src_dir ${PROJECT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR})
      install(FILES ${headers} DESTINATION ${CNOID_HEADER_SUBDIR}/cnoid/${rel_src_dir})
    endif()
  endif()
endfunction()

add_subdirectory(src)