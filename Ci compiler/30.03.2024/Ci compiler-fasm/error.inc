error.winapi:
; eax - name func
; ebx - error code
invoke printf, error.textWinAPI, eax, ebx, ebx
invoke getch
invoke ExitProcess, 1

error.lexer:
; eax - adr text
invoke printf, error.textLexer, eax
invoke getch
invoke ExitProcess, 1