let g:cpp_plugin.indexer.builder.autoBuild = 1
call g:cpp_plugin.indexer.builder.addCustomRegex('c++', '/[ \t]*using[ \t]*([A-Za-z0-9_]*)[ \t]*=/\1/t/')
call g:cpp_plugin.indexer.builder.addCustomRegex('python', '/[ \t]*self\.([A-Za-z0-9_]*)[ \t]*=/\1/v/')

call g:buildsystem.setAvailableBuildConfigs( { 'host': CMakeBuildConfig(4, './build/') } )

set path+=core
let g:include_directories = [ 'core' ]

function! GetCppNamespaceFromPath(path)
	let res = []
	if len(a:path) > 1 && a:path[0] == 'core'
		if len(a:path) > 2 && a:path[1] == 'joint'
			call add(res, 'joint')
			if len(a:path) > 3 && a:path[2] == 'detail'
				call add(res, 'detail')
			endif
		endif
	endif
	return res
endf

if exists('g:c_std_includes') && exists('g:cpp_std_includes') && exists('g:platform_includes')
	let g:include_priorities = [ 'joint/.*', 'benchmarks/.*', 'cxxtest/.*', g:platform_includes, 'boost/.*', g:cpp_std_includes, g:c_std_includes ]
end
