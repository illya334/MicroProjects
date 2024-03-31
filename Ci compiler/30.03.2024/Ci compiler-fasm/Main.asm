format PE Console 4.0
entry Start

include 'win32a.inc'

section '.data' data readable writeable

include 'data.inc'

hello dw 'h', 'e', 'l', 'l', 'o', ' ', 'w', 'o', 'r', 'l', 'd', 0
world dw 'w', 'o', 'r', 'l', 'd', 0
  
section '.text' code readable executable

;proc func, param:abc

;  mov eax, [param.a]
;  mov ebx, [param.b]
;  mov ecx, [param.c]

;ret
;endp

include 'filer.inc'
include 'lexer.inc'
include 'string.inc'

error.func:
; eax - name func
; ebx - error code
invoke printf, error.text, eax, ebx, ebx
invoke getch
invoke ExitProcess, 1

Start:

  stdcall wstrfind, hello, world

  invoke LocalAlloc, LPTR, 20
  mov [tmpMem.pMem], eax
  mov [tmpMem.len], 20
  
  invoke LocalHandle, eax
  mov [tmpMem.hMem], eax


  stdcall openFile, fileInName, fileIn
  
  stdcall readFile, fileIn 
  
  stdcall lexer   

  invoke printf, text, eax, [fileIn.pMem] 
  invoke getch

Exit:
  invoke  ExitProcess, 0

section '.idata' import data readable

  library kernel32, 'KERNEL32.DLL',\
          msvcrt, 'msvcrt.dll'

  include 'api/kernel32.inc'
          
  import msvcrt,\
          printf, 'printf',\
          scanf, 'scanf',\
          getch, '_getch'