if exists("b:current_syntax")
    finish
endif

syn match importPackage /.*/ contained
syn keyword importStatement import nextgroup=importPackage skipwhite

syn match packageName /\<[A-Za-z0-9.]\+\>/ contained
syn keyword packageDecl package nextgroup=packageName skipwhite skipempty

syn match interfaceName /\<\i\+\>/ contained
syn keyword interfaceKeyword interface contained
syn region interfaceDeclBody start=// end=/}/ contains=methodDecl contained keepend
syn region interfaceDeclBegin start=/\<interface\>/ end=/{/ contains=interfaceKeyword,interfaceName,baseInterfacesList nextgroup=interfaceDeclBody keepend
syn region interfaceDecl start=/\<interface\>/ end=/}/ contains=interfaceDeclBegin,interfaceDeclBody keepend

syn match baseInterfaceName /\<\i\+\>/ contained
syn match baseInterfacePackage /\<[A-Za-z0-9.]\+\>\./ contained nextgroup=baseInterfaceName
syn region baseInterfacesList start=/:/ end=/{/ contains=baseInterfacePackage,baseInterfaceName

syn region methodDecl start=/\i/ end=/;/ contains=retTypePackage,retTypeName,methodName contained keepend

syn region methodArgs start=/(/ end=/)/ contained contains=argTypePackage,argTypeName,argName
syn match methodName /\<\i\+\>/ contained nextgroup=methodArgs skipwhite skipempty

syn match argName /\<\i\+\>/ contained
syn region argTypeArrayModifier start=/\[/ end=/\]/ contained nextgroup=argName skipwhite skipempty
syn match argTypeName /\<\i\+\>/ contained nextgroup=argName,argTypeArrayModifier skipwhite skipempty
syn match argTypePackage /\<[A-Za-z0-9.]\+\>\./ contained nextgroup=argTypeName

syn region retTypeArrayModifier start=/\[/ end=/\]/ contained nextgroup=methodName skipwhite skipempty
syn match retTypeName /\<\i\+\>/ contained nextgroup=methodName,retTypeArrayModifier skipwhite skipempty
syn match retTypePackage /\<[A-Za-z0-9.]\+\>\./ contained nextgroup=retTypeName

syn match structName /\<\i\+\>/ contained
syn keyword structKeyword struct contained
syn region structDeclBody start=// end=/}/ contains=fieldDecl contained keepend
syn region structDeclBegin start=/\<struct\>/ end=/{/ contains=structKeyword,structName nextgroup=structDeclBody keepend
syn region structDecl start=/\<struct\>/ end=/}/ contains=structDeclBegin,structDeclBody keepend

syn region fieldDecl start=/\i/ end=/;/ contains=fieldTypePackage,fieldTypeName,fieldName contained keepend

syn match fieldName /\<\i\+\>/ contained
syn region fieldTypeArrayModifier start=/\[/ end=/\]/ contained nextgroup=fieldName skipwhite skipempty
syn match fieldTypeName /\<\i\+\>/ contained nextgroup=fieldName,fieldTypeArrayModifier skipwhite skipempty
syn match fieldTypePackage /\<[A-Za-z0-9.]\+\>\./ contained nextgroup=fieldTypeName

syn match enumName /\<\i\+\>/ contained
syn keyword enumKeyword enum contained
syn region enumDeclBody start=// end=/}/ contains=enumElementName,enumElementValue contained keepend
syn region enumDeclBegin start=/\<enum\>/ end=/{/ contains=enumKeyword,enumName nextgroup=enumDeclBody keepend
syn region enumDecl start=/\<enum\>/ end=/}/ contains=enumDeclBegin,enumDeclBody keepend

syn match enumElementName /\<[A-Za-z]\i*\>/ contained
syn match enumElementValue /\<\d\+\>/ contained


hi def link importStatement Statement
hi def link importPackage String

hi def link packageDecl Statement
hi def link packageName PreProc

hi def link interfaceKeyword Statement
hi def link interfaceName Type

hi def link baseInterface Type
hi def link baseInterfacePackage PreProc
hi def link baseInterfaceName Type

hi def link methodName Function

hi def link retTypePackage PreProc
hi def link retTypeName Type

hi def link argTypePackage PreProc
hi def link argTypeName Type
hi def link argName Variable

hi def link structKeyword Statement
hi def link structName Type

hi def link fieldTypePackage PreProc
hi def link fieldTypeName Type
hi def link fieldName Variable

hi def link enumKeyword Statement
hi def link enumName Type
hi def link enumElementName Variable
hi def link enumElementValue Number
