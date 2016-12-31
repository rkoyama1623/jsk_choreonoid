message("This is boost python")

set(Boost_PYTHON_PREFIX ${CATKIN_DEVEL_PREFIX}/boost)
set(Boost_PYTHON_INCLUDE_DIRS ${Boost_PYTHON_PREFIX}/include)
set(Boost_PYTHON_LIB_DIRS ${Boost_PYTHON_PREFIX}/lib)
set(Boost_PYTHON_PREFIX ${CATKIN_DEVEL_PREFIX}/boost PARENT_SCOPE)
set(Boost_PYTHON_INCLUDE_DIRS ${Boost_PYTHON_PREFIX}/include PARENT_SCOPE)
set(Boost_PYTHON_LIB_DIRS ${Boost_PYTHON_PREFIX}/lib PARENT_SCOPE)
message(Boost_PYTHON_PREFIX= ${Boost_PYTHON_PREFIX}) # catkin_ws/devel/.private/boost_ros_pkg
message(Boost_PYTHON_INCLUDE_DIRS= ${Boost_PYTHON_INCLUDE_DIRS})

# build boost from source
execute_process(COMMAND make -f ${PROJECT_SOURCE_DIR}/3rdparty/boost/Makefile.boost PREFIX=${Boost_PYTHON_PREFIX} install.boost MODULE=python)
