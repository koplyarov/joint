joint_generate_adapters(python Benchmarks_adapters.py ../idl/IBenchmarks.idl)

add_custom_target(benchmarks-adapters-py ALL DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/Benchmarks_adapters.py)

