format PE Console 4.0
entry Start

include 'win32a.inc'

section '.data' data readable writeable

include 'data.inc'
  
section '.text' code readable executable

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

  stdcall openFile, fileInName, fileIn
  
  stdcall readFile, fileIn 
  
  _alloc 40
  mov [tmpMem.pMem], eax
  
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