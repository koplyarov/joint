project(test-c)

joint_generate_adapters(python ${CMAKE_CURRENT_SOURCE_DIR}/Benchmarks_adapters.py ../idl/IBenchmarks.idl)

add_custom_target(benchmarks-adapters-py ALL DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/Benchmarks_adapters.py)

