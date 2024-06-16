; Тут збурігаються змінні

; defines
  true equ 1
  false equ 0

; global
error:
  .textWinAPI   db "ERROR: In function '%s', code: 0x%X (%d);", 10, 13, 0
  .textLexer    db "ERROR LEXER: %s", 10, 13, 0
  .CreateFileA  db 'CreateFileA', 0
  .ReadFile     db 'ReadFile', 0
  .waitName     db 'Invalid command - name expected', 0
    .fileEnd    db 'File ended', 0
  .waitValue    db 'Invalid command - value expected', 0
  
text db 'Read: %d; Text: %s',0

tmpMem:
  .hMem dd 0
  .pMem dd 0
  .len  dd 40

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
datatype db   'void',0,\   ; 0
              'byte',0,\   ; 1
              'word',0,\   ; 2
              'dword',0,\  ; 3
              'qword',0,\  ; 4
              'char',0,\   ; 5
              'wchar',0,\  ; 6
              'short',0,\  ; 7
              'int',0,\    ; 8
              'long',0,\   ; 9
              'float',0,\  ; 10
              'double',0,\ ; 11
              'bool',0,\   ; 12
              'file',0,0   ; 13 ; 14 - none
datatype.len = $ - datatype
              
simbols db    'ABCDEFGIHJKLMNOPQRSTUVWXYZabcdefgihjklmnopqrstuwyx0123456789_.',0
simbols.len = $ - simbols

; now comand data
nowcom:
  .type db 0
  .datatype db 0
  .name dd 0 ; adr
  .value dd 0 ; adr

; TMP VARs
tmpVar:
  .a dd 0

nowLine dd 0

; Виділенна пам'ять для токенів  
pTocken dd 0 
tockenIndex dd 0

; struct stTocken{
;   db type
;   db datatype

;   dw size name
;   db [size] name

;   dw size value
;   db [size] value  
; }