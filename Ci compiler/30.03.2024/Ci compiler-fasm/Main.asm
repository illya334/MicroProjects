; By zeq52giw 25.03.2024

; Ci compiler

; ������� ���� - ���������� Ci ���� � ��������� (FASM)

; �� ������ ��������� ��� ��������� �� ��������� FASM.

; TODO - ���'����, �� �������.
; LTDI - lazy to do it (�������, ��� ������� ��) - �������� ���� ������� ��� ���� �� ���� �� ����� ����������� ��������, ��� �� ��������� ��� ���� ������ ����� �� �������� ��,
;         ��� ����� ��, �� ����� ��� ���� ��������� �'��� - ��� �����������, ������� ������ ������� ������.

; ��������� �� LTDI:
;  lexer.inc: createTocken

; TODO: ������� ���� ���� ����������, ��������� ������� ������ - �� ��������� ��� ��������� ������� �������� ����������, ��� ����� �� ���� ���������� �� ���� ��������.

; FATAL ERROR: ������� ������� ��� ����������� ������� ���'�� � createTocken! 

format PE Console 4.0
entry Start

include 'win32a.inc'

include 'structs.inc'


section '.data' data readable writeable

include 'data.inc'

section '.text' code readable executable


include 'filer.inc'
include 'string.inc'
include 'lexer.inc'
include 'error.inc'

Start:

  stdcall openFile, fileInName, fileIn
  stdcall readFile, fileIn
  stdcall closeFile, fileIn

  nop
  stdcall lexer

  ;invoke printf, text, eax, [fileIn.pMem]
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