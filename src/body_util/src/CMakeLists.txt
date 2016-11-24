link_directories(${choreonoid_LIBRARY_DIRS})

add_cnoid_python_module(sample sample.cpp)
target_link_libraries(sample CnoidBody CnoidPython ${PYTHON_LIBRARIES} ${Boost_LIBRARIES})


# target_link_libraries(sample CnoidBody CnoidPython ${PYTHON_LIBRARIES} ${Boost_LIBRARIES} ${choreonoid_LIBRARIES})
# target_link_libraries(sample ${PYTHON_LIBRARIES} ${Boost_LIBRARIES})

# add_library(hello_ext SHARED hello_ext.cpp)
# target_link_libraries(hello_ext )
# set_target_properties(hello_ext PROPERTIES
#   LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib
#   PREFIX ""
#   SUFFIX ".so")

