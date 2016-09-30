let g:cpp_plugin.indexer.builder.autoBuild = 1
call g:cpp_plugin.indexer.builder.addCustomRegex('c++', '/[ \t]*using[ \t]*([A-Za-z0-9_]*)[ \t]*=/\1/t/')
call g:cpp_plugin.indexer.builder.addCustomRegex('python', '/[ \t]*self\.([A-Za-z0-9_]*)[ \t]*=/\1/v/')

call g:buildsystem.setAvailableBuildConfigs( { 'host': CMakeBuildConfig(4, './build/') } )

set path+=joint
set path+=bindings/python
set path+=bindings/cpp
set path+=bindings/java
set path+=build/test
set path+=build/benchmarks
let g:include_directories = [ 'joint', 'bindings/cpp', 'bindings/java', 'bindings/python', 'build', 'benchmarks/core', 'benchmarks' ]

function! GetCppNamespaceFromPath(path)
	let res = []
	if len(a:path) > 1 && a:path[0] == 'joint'
		if len(a:path) > 2 && a:path[1] == 'joint'
			call add(res, 'joint')
			if len(a:path) > 3 && (a:path[2] == 'detail' || a:path[2] == 'devkit')
				call add(res, a:path[2])
			endif
		endif
	endif
	if len(a:path) > 1 && a:path[0] == 'bindings'
		if len(a:path) > 2 && a:path[1] == 'cpp'
			call add(res, 'joint')
			if len(a:path) > 4 && a:path[3] == 'detail'
				call add(res, 'detail')
			endif
		endif
	endif
	return res
endf

if exists('g:c_std_includes') && exists('g:cpp_std_includes') && exists('g:platform_includes')
	let g:include_priorities = [ 'joint/.*', 'benchmarks/.*', 'cxxtest/.*', g:platform_includes, 'boost/.*', g:cpp_std_includes, g:c_std_includes ]
end

match Error /^.*\S *\zs\t\+/
