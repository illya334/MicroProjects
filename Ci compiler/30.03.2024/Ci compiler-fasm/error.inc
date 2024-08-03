;    /  -----------  \
;  /      zeq52giw     \
; <   ---------------   >
;  \                  /
;    \  -----------  /

; By zeq52giw 30.03.2024

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

error.getLastError:
invoke GetLastError
invoke printf, error.testErrorText, eax
invoke getch
ret
