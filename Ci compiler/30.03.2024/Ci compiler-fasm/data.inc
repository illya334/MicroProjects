; Тут збурігаються змінні

; global
error:
  .text         db "ERROR: In function '%s', code: 0x%X (%d);", 10, 13, 0
  .CreateFileA  db 'CreateFileA', 0
  .ReadFile     db 'ReadFile', 0
  
text db 'Read: %d; Text: %s',0

tmpMem:
  .hMem dd 0
  .pMem dd 0
  .len  dd 0

; filer.inc
fileInName db 'C:\proj\testIn.txt', 0
fileOutName db '', 0

fileIn:
  .hFile    dd 0 ; 0
  .hMem     dd 0 ; 1 Handle memory
  .pMem     dd 0 ; 2 Point on memory
  .lenMem   dd 0 ; 3
  .fileSize dd 0 ; 4
  .index    dd 0 ; 5

fileOut:
  .hFile    dd 0
  .hMem     dd 0
  .pMem     dd 0
  .lenMem   dd 0
  .fileSize dd 0
  .index    dd 0  
                                        
string:
  .nl db 10, 13, 0
  .d db '%d', 0
  .f db '%f', 0
  .x db '0x%X', 0

cnt dd 0

; lexer.inc
datatype db   'void',0,\
              'byte',0,\
              'word',0,\
              'dword',0,\
              'qword',0,\
              'char',0,\
              'wchar',0,\
              'short',0,\
              'int',0,\
              'long',0,\
              'float',0,\
              'double',0,\
              'bool',0,\
              'file',0,0
datatype.len = $ - datatype
              
simbols db    'ABCDEFGIHJKLMNOPQRSTUVWXYZabcdefgihjklmnopqrstuwyx0123456789_',0
simbols.len = $ - simbols