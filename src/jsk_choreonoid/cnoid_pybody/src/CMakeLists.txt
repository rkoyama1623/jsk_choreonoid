link_directories(${choreonoid_LIBRARY_DIRS} ${Boost_PYTHON_LIB_DIR})

add_jsk_choreonoid_python_module(cnoid_body_sample cnoid_body_sample.cpp)
target_link_libraries(cnoid_body_sample CnoidBody CnoidPython ${PYTHON_LIBRARIES} ${Boost_PYTHON_LIBRARIES})

add_jsk_choreonoid_python_module(PyBodyMethods python/PyBodyMethods.cpp)
target_link_libraries(PyBodyMethods CnoidBody CnoidPython ${PYTHON_LIBRARIES} ${Boost_PYTHON_LIBRARIES})

add_jsk_choreonoid_python_module(cnoid_numpy_sample cnoid_numpy_sample.cpp)
target_link_libraries(cnoid_numpy_sample
  CnoidBody CnoidPython ${PYTHON_LIBRARIES} ${Boost_PYTHON_LIBRARIES} ${choreonoid_PLUGINDIR}/python/cnoid/Util.so)

add_jsk_choreonoid_python_module(numpy_array_sample numpy_array_sample.cpp)
target_link_libraries(numpy_array_sample ${Boost_PYTHON_LIBRARIES} ${PYTHON_LIBRARIES}) # ${Boost_LIBRARIES}
