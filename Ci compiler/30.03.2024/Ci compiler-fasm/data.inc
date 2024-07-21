; By zeq52giw 25.03.2024

; Тут зберігаються змінні

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
  .invalidSim   db 'Invalid simbol', 0

text db 'Read: %d; Text: %s',0

tmpMem:
  .hMem dd 0
  .pMem dd 0
  .len  dd 40

; filer.inc
fileInName db 'C:\proj\testIn.txt', 0
fileOutName db '', 0

fileIn stFile

string:
  .nl db 10, 13, 0
  .d db '%d', 0
  .f db '%f', 0
  .x db '0x%X', 0

cnt dd 0

; lexer.inc
datatype db   'void',0,\   ; 1 ; 0 - none
              'byte',0,\   ; 2
              'word',0,\   ; 3
              'dword',0,\  ; 4
              'qword',0,\  ; 5
              'char',0,\   ; 6
              'wchar',0,\  ; 7
              'short',0,\  ; 8
              'int',0,\    ; 9
              'long',0,\   ; 10
              'float',0,\  ; 11
              'double',0,\ ; 12
              'bool',0,\   ; 13
              'file',0,0   ; 14 ; 15 - none
datatype.len = $ - datatype

simbols db    'ABCDEFGIHJKLMNOPQRSTUVWXYZabcdefgihjklmnopqrstuvwyx0123456789_',0
simbols.len = $ - simbols

; now comand data
nowcom dd 0
  nowcom.type = 0
  nowcom.datatype = 1
  nowcom.name = 2
  nowcom.value = 6
  nowcom.params = 10
nowcom.size = 14
  ;.type db 0         ; 0 - offset
  ;.datatype db 0     ; 1
  ;.name dd 0 ; adr   ; 2
  ;.value dd 0 ; adr  ; 6
  ;.params dd 0 ; adr ; 10

; TYPE:
; 0 - NONE
; 1 - Create var ( int a; )
; 2 - Create var with value ( int a = 10; )

nowLine dd 0
nowCol dd 0

; Виділенна пам'ять для токенів
pTocken dd 0
tockenIndex dd 0

tockenText db 'Tocken = { type: %d; datatype: %d; name: "%s"; value: "%s"; }', 10, 13, 0

; struct stTocken{
;   db type
;   db datatype

;   dw size name
;   db [size] name

;   dw size value
;   db [size] value
; }