set tabstop=4
set shiftwidth=4
set expandtab

let joint_root = expand('<sfile>:p:h')
exec 'set runtimepath+='.joint_root.'/ide/vim'

au BufNewFile,BufRead *.jm set ft=json
au BufNewFile,BufRead *.idl set ft=joint

set path+=core
set path+=devkit
set path+=bindings/python
set path+=bindings/cpp
set path+=bindings/java
set path+=build/test
set path+=build/benchmarks
