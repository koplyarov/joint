let g:cpp_plugin.indexer.builder.autoBuild = 1
call g:cpp_plugin.indexer.builder.addCustomRegex('c++', '/[ \t]*BEGIN_CLANG_WRAPPER_NO_DISPOSE[ \t]*\(([A-Za-z0-9_]*)\)/\1/t/')
call g:cpp_plugin.indexer.builder.addCustomRegex('python', '/[ \t]*self\.([A-Za-z0-9_]*)[ \t]*=/\1/v/')
