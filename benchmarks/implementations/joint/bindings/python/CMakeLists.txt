joint_generate_adapters(python Benchmarks_adapters.py
	${BENCHMARKS_IDL_DIR}/IBasicBenchmarks.idl
	${BENCHMARKS_IDL_DIR}/ICastBenchmarks.idl
	${BENCHMARKS_IDL_DIR}/IEnumBenchmarks.idl
	${BENCHMARKS_IDL_DIR}/IExceptionBenchmarks.idl
	${BENCHMARKS_IDL_DIR}/IObjectBenchmarks.idl
	${BENCHMARKS_IDL_DIR}/IStructBenchmarks.idl
)

add_custom_target(benchmarks-adapters-py ALL DEPENDS Benchmarks_adapters.py)

joint_make_module_package(benchmarks-component-py
	PACKAGE_DIR ${JOINT_MODULE_OUTPUT_DIRECTORY}/python/Benchmarks
	MANIFEST Benchmarks.jm
	DEPENDS benchmarks-adapters-py
)

joint_add_module_content(benchmarks-component-py
	DIRECTORY Package
	FILES __init__.py Component.py ${CMAKE_CURRENT_BINARY_DIR}/Benchmarks_adapters.py
)
